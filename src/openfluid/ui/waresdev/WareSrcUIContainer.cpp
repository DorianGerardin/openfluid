/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

 */

/**
  @file WareSrcUIContainer.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#include <QCoreApplication>
#include <QMessageBox> // HACk

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/waresdev/WareSrcUIContainer.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcUIContainer::WareSrcUIContainer(const QString& AbsolutePath, 
                                       openfluid::ware::WareType Type, const QString& WareID):
  QObject(),
  openfluid::waresdev::WareSrcContainer(AbsolutePath.toStdString(),Type, WareID.toStdString()), 
  mp_Process(new WareSrcProcess()),mp_CurrentParser(new openfluid::waresdev::WareSrcMsgParserGcc())
{
  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutput()));

  connect(mp_Process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedOutput(int)));
}


// =====================================================================
// =====================================================================


WareSrcUIContainer::~WareSrcUIContainer()
{
  if (mp_Process->state() != WareSrcProcess::NotRunning)
  {
    mp_Process->close();
  }

  delete mp_Process;
  delete mp_CurrentParser;
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processStandardOutput()
{
  mp_Process->setReadChannel(WareSrcProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    auto Message =
        mp_CurrentParser->parse(MsgLine,openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD);

    mp_Stream->write(Message);

    if (Message.m_Type != openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD)
    {
      m_Messages.append(Message);
    }
  }

}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processErrorOutput()
{
  mp_Process->setReadChannel(WareSrcProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString MsgLine = QString::fromUtf8(mp_Process->readLine());

    auto Message =
        mp_CurrentParser->parse(MsgLine,openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_WARNING);

    mp_Stream->write(Message);

    m_Messages.append(Message);
  }
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::processFinishedOutput(int ExitCode)
{
  QString ElapsedString =
      QString("execution time : %1").arg(QString::fromStdString(m_ProcessTimer.elapsedAsPrettyString()));

  if (!ExitCode)
  {
    auto Message =
        openfluid::waresdev::WareSrcMsgParser::WareSrcMsg(
          QString("\nCommand ended (%1)\n\n").arg(ElapsedString),
                  openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
    mp_Stream->write(Message);
  }
  else
  {
    auto Message = 
      openfluid::waresdev::WareSrcMsgParser::WareSrcMsg(
        QString("\nCommand ended with error (%1)\n\n").arg(ElapsedString),
        openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_ERROR);
    mp_Stream->write(Message);
  }


  emit processFinished();

  if (mp_Process->getType() == WareSrcProcess::Type::CONFIGURE)
  {
    emit configureProcessFinished(m_Type,QString::fromStdString(m_ID));
  }
  else if (mp_Process->getType() == WareSrcProcess::Type::BUILD)
  {
    emit buildProcessFinished(m_Type,QString::fromStdString(m_ID));
  }

  mp_Process->setType(WareSrcProcess::Type::NONE);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::configure()
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "CMake program not available");
  }


  mp_Stream->clear();
  m_Messages.clear();


  prepareBuildDirectory();

  delete mp_CurrentParser;
  mp_CurrentParser = new openfluid::waresdev::WareSrcMsgParserCMake(getAbsolutePath());


  // === build and run command

  QStringList ExtraOptionsList = 
      openfluid::tools::convertArgsStringToList(getConfigureExtraOptions());

  openfluid::utils::CMakeProxy::CommandInfos CmdInfos = 
    openfluid::utils::CMakeProxy::getConfigureCommand(QString::fromStdString(m_BuildDirPath),getAbsolutePath(),
                                                      getConfigureVariables(), getConfigureGenerator(),
                                                      ExtraOptionsList);

  runCommand(CmdInfos, getConfigureEnvironment(), WareSrcProcess::Type::CONFIGURE);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::build()
{
  if (!openfluid::utils::CMakeProxy::isAvailable())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "CMake program not available");
  }


  mp_Stream->clear();
  m_Messages.clear();


  // run configure if build dir does not exist
  if (!openfluid::tools::FilesystemPath(m_BuildDirPath).exists())
  {
    configure();

    while (!mp_Process->waitForFinished(200))  // TODO better to replace this by a threaded process
    {
      qApp->processEvents();
    }

    mp_Stream->write(
      openfluid::waresdev::WareSrcMsgParser::WareSrcMsg(
        "\n================================================================================\n\n\n",
        openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND
      )
    );
  }


  QString Target = getBuildTarget();


  delete mp_CurrentParser;
  mp_CurrentParser = new openfluid::waresdev::WareSrcMsgParserGcc();


  // === build and run command

  openfluid::utils::CMakeProxy::CommandInfos CmdInfos =
    openfluid::utils::CMakeProxy::getBuildCommand(QString::fromStdString(m_BuildDirPath),Target,m_BuildJobs);

  runCommand(CmdInfos, getBuildEnvironment(), WareSrcProcess::Type::BUILD);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::generateDoc()
{
  // TOIMPL
  QMessageBox::critical(nullptr,"Generate doc","not implemented",QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void WareSrcUIContainer::runCommand(const openfluid::utils::CMakeProxy::CommandInfos& CmdInfos, 
                                    const QProcessEnvironment& Env, WareSrcProcess::Type CmdType)
{
  if (mp_Process->state() != WareSrcProcess::NotRunning)
  {
    mp_Process->close();
  }

  m_ProcessTimer.restart();

  mp_Process->setType(CmdType);

  emit processLaunched();

  if (CmdType == WareSrcProcess::Type::CONFIGURE)
  {
    emit configureProcessLaunched(m_Type,QString::fromStdString(m_ID));
  }
  else if (CmdType == WareSrcProcess::Type::BUILD)
  {
    emit buildProcessLaunched(m_Type,QString::fromStdString(m_ID));
  }

  if (openfluid::base::PreferencesManager::instance()->isWaresdevShowCommandEnv("PATH"))
  {
    auto PATHMessage =
        openfluid::waresdev::WareSrcMsgParser::WareSrcMsg(
          QString("PATH=%1\n").arg(Env.value("PATH", "")),
          openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
    mp_Stream->write(PATHMessage);
  }

  auto CommandMessage =
      openfluid::waresdev::WareSrcMsgParser::WareSrcMsg(
        QString("%1\n").arg(CmdInfos.joined()),
        openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND);
  mp_Stream->write(CommandMessage);

  mp_Process->setProcessEnvironment(Env);
  mp_Process->start(CmdInfos.Program,CmdInfos.Args);
}


// =====================================================================
// =====================================================================


QProcessEnvironment WareSrcUIContainer::getConfigureEnvironment() const
{
  // === set process environment
  QProcessEnvironment Env = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = 
    QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevConfigureEnv("PATH"));
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      Env.insert("PATH", CustomPath);
    }
  }

  return Env;
}


// =====================================================================
// =====================================================================


QProcessEnvironment WareSrcUIContainer::getBuildEnvironment() const
{
  QProcessEnvironment RunEnv = QProcessEnvironment::systemEnvironment();

  // Set PATH env. var. if configured
  QString CustomPath = 
    QString::fromStdString(openfluid::base::PreferencesManager::instance()->getWaresdevBuildEnv("PATH"));
  if (!CustomPath.isEmpty())
  {
    QByteArray ExistingPath = qgetenv("PATH");
    if (!ExistingPath.isNull())
    {
      CustomPath.replace("%%PATH%%", ExistingPath);
      RunEnv.insert("PATH", CustomPath);
    }
  }

  return RunEnv;
}


// =====================================================================
// =====================================================================


QList<openfluid::waresdev::WareSrcMsgParser::WareSrcMsg> WareSrcUIContainer::getMessages()
{
  return m_Messages;
}


// =====================================================================
// =====================================================================


bool WareSrcUIContainer::isProcessRunning() const
{
  return (mp_Process->state() != WareSrcProcess::NotRunning);
}


} } }  // namespaces
