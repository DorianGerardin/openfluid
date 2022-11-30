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
 @file GitUIProxy.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QDir>
#include <QTextStream>

#include <openfluid/ui/waresdev/GitUIProxy.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


GitUIProxy::GitUIProxy() : openfluid::utils::GitProxy()
{
  // TODO use a subdir in tmp dir given by openfluid::tools::Filesystem::makeUniqueSubdirectory()
  m_TmpPath = QString::fromStdString(openfluid::base::Environment::getTempDir());
}


// =====================================================================
// =====================================================================


GitUIProxy::~GitUIProxy()
{
  delete mp_Process;
}


// =====================================================================
// =====================================================================


QString GitUIProxy::getCurrentOpenFLUIDBranchName()
{
  return "openfluid-" + QString::fromStdString(openfluid::base::Environment::getVersionMajorMinor());
}


// =====================================================================
// =====================================================================


void GitUIProxy::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    emit info(QString::fromUtf8(mp_Process->readLine()));
  }
}


// =====================================================================
// =====================================================================


void GitUIProxy::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString Msg = QString::fromUtf8(mp_Process->readLine());
    emit error(Msg);
  }
}


// =====================================================================
// =====================================================================


void GitUIProxy::processErrorOutputAsInfo()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    QString Msg = QString::fromUtf8(mp_Process->readLine());
    emit info(Msg);
  }
}


// =====================================================================
// =====================================================================


bool GitUIProxy::launchCommand(QStringList Args, const QString& FromUrl, const QString& ToPath,
                               const QString& Username, const QString& Password,
                               bool SslNoVerify, const QString& WorkingDirectory, bool UsernameViaEnv)
{
  if (FromUrl.isEmpty() || ToPath.isEmpty())
  {
    emit error(tr("Empty remote url or empty destination path"));
    return false;
  }

  // Manual check if .git/index.lock detected (meaning that another git process is already running)
  // HACK see if removable for release
  if (!WorkingDirectory.isEmpty())
  {
    openfluid::tools::FilesystemPath GitIndexLockPath = 
          openfluid::tools::FilesystemPath({WorkingDirectory.toStdString(), ".git", "index.lock"});
    if (GitIndexLockPath.exists())
    {
      emit error(tr("Can not operate, git lock detected."));
      return false;
    }
  }

  QString Url = FromUrl;
  QStringList Options;

  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();

  if (!Username.isEmpty() && !Url.contains("@") && !UsernameViaEnv)
  {
    Url.replace("http://", QString("http://%1@").arg(Username));
    Url.replace("https://", QString("https://%1@").arg(Username));
  }

  if (!Password.isEmpty())
  {
    PcsEnv.insert("GIT_ASKPASS",QString::fromStdString(openfluid::config::GITASKPASS_APP));
    PcsEnv.insert(OFBUILD_GITASKPASS_ENVVAR,Password);
    if (UsernameViaEnv)
    {
      PcsEnv.insert(OFBUILD_GITASKUSER_ENVVAR,Username);
    }
  }

  if (SslNoVerify)
  {
    Options.append("-c");
    Options.append("http.sslverify=false");
  }

  Args.append(Options);
  Args.append(Url);
  Args.append(ToPath);

  mp_Process = new QProcess();
  if (!WorkingDirectory.isEmpty())
  {
    mp_Process->setWorkingDirectory(WorkingDirectory);
  }
  mp_Process->setProcessEnvironment(PcsEnv);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutputAsInfo()));

  mp_Process->start(QString::fromStdString(m_ExecutablePath),Args);

  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);

  QString Res = QString::fromUtf8(mp_Process->readAll());
  int ErrCode = mp_Process->exitCode();

  delete mp_Process;
  mp_Process = nullptr;

  return (ErrCode == 0);
}


// =====================================================================
// =====================================================================


bool GitUIProxy::addSubmodule(const QString& FromUrl, const QString& ToPath, const QString& LocalGitRepoPath,
                              const QString& Username, const QString& Password,
                              bool SslNoVerify)
{
  QStringList Args = {"submodule", "add", "--progress"};
  return launchCommand(Args, FromUrl, ToPath, Username, Password, SslNoVerify, LocalGitRepoPath, true);
}


// =====================================================================
// =====================================================================


bool GitUIProxy::clone(const QString& FromUrl, const QString& ToPath,
                       const QString& Username, const QString& Password,
                       bool SslNoVerify, const QString& LocalGitRepoPath, bool WithoutVersioning)
{
  QStringList Args = {"clone", "--recurse-submodules", "--progress"};
  if (WithoutVersioning)
  {
    Args << "--depth=1";
  }
  return launchCommand(Args, FromUrl, ToPath, Username, Password, SslNoVerify, LocalGitRepoPath, true);
}


// =====================================================================
// =====================================================================


GitUIProxy::TreeStatusInfo GitUIProxy::status(const QString& Path)
{
  TreeStatusInfo TreeStatus;
  QDir PathDir(Path);

  if (!PathDir.exists(".git"))
  {
    return TreeStatus;
  }

  TreeStatus.m_IsGitTracked = true;

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  QStringList Args = {"status","--porcelain","--ignored","-b"};

  mp_Process->start(QString::fromStdString(m_ExecutablePath),Args);

  mp_Process->waitForReadyRead(-1);
  mp_Process->waitForFinished(-1);

  QString Out = mp_Process->readAll();

  TreeStatus.m_BranchName = Out.section('\n', 0, 0).section(' ', 1).section("...", 0, 0);

  QStringList PathLines = Out.section('\n', 1).split('\n', QString::SkipEmptyParts);
  for (const QString& PathLine : PathLines)
  {
    QChar IndexLetter = PathLine.at(0);
    QChar WorkTreeLetter = PathLine.at(1);

    QString FilePath = PathLine;
    FilePath.remove(0, 3);

    FileStatusInfo FilePathStatus;

    if ((IndexLetter == 'D' && WorkTreeLetter == 'D') || (IndexLetter == 'A' && WorkTreeLetter == 'A')
        || IndexLetter == 'U'
        || WorkTreeLetter == 'U')
    {
      FilePathStatus.m_IndexStatus = FileStatus::CONFLICT;
    }
    else if (IndexLetter == '?')
    {
      FilePathStatus.m_IndexStatus = FileStatus::UNTRACKED;
    }
    else if (IndexLetter == '!')
    {
      FilePathStatus.m_IndexStatus = FileStatus::IGNORED;
    }
    else
    {
      if (WorkTreeLetter == 'M')
      {
        FilePathStatus.m_IsDirty = true;
      }

      if (IndexLetter == 'M')
      {
        FilePathStatus.m_IndexStatus = FileStatus::MODIFIED;
      }
      if (IndexLetter == 'A')
      {
        FilePathStatus.m_IndexStatus = FileStatus::ADDED;
      }
      // Deleted takes precedence over Added
      if (IndexLetter == 'D' || WorkTreeLetter == 'D')
      {
        FilePathStatus.m_IndexStatus = FileStatus::DELETED;
      }
    }

    TreeStatus.m_FileStatusByTreePath[FilePath] = FilePathStatus;
  }

  delete mp_Process;
  mp_Process = nullptr;

  return TreeStatus;
}


// =====================================================================
// =====================================================================


std::pair<int, QString> GitUIProxy::commandHtml(const QString& Path, QStringList Args, bool RequiringGit)
{
  QDir PathDir(Path);

  if (RequiringGit && !PathDir.exists(".git"))
  {
    return std::pair(-1, QString());
  }

  mp_Process = new QProcess();
  mp_Process->setWorkingDirectory(Path);

  mp_Process->start(QString::fromStdString(m_ExecutablePath),Args);

  mp_Process->waitForReadyRead(-1);
  mp_Process->waitForFinished(-1);

  QString Res = QString::fromUtf8(mp_Process->readAll());
  int ExitCode = mp_Process->exitCode();

  delete mp_Process;
  mp_Process = nullptr;

  return std::pair(ExitCode, Res);
}


// =====================================================================
// =====================================================================


QString GitUIProxy::statusHtml(const QString& Path, bool WithColorCodes)
{

  QStringList Args;

  if (WithColorCodes)
  {
    Args << "-c" << "color.status=always";
  }

  Args << "status";

  return commandHtml(Path, Args).second;
}


// =====================================================================
// =====================================================================


QString GitUIProxy::logHtml(const QString& Path, bool WithColorCodes)
{

  QStringList Args;
  Args << "log";

  if (WithColorCodes)
  {
    Args << "--color";
  }

  return commandHtml(Path, Args).second;
}


// =====================================================================
// =====================================================================


 QString GitUIProxy::init(const QString& Path)
{
  QStringList Args;
  Args << "init";
  return commandHtml(Path, Args, false).second;
}


} } } // namespaces