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
 \file WareSrcExplorerModel.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */


#include "WareSrcExplorerModel.hpp"

#include <QFont>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>

// =====================================================================
// =====================================================================


WareSrcExplorerModel::WareSrcExplorerModel(const QString& Path) :
    QFileSystemModel(), m_DirPath(Path)
{
  m_DefaultIcons["*.c++;*.cxx;*.cpp;*.cc;*.C;*.moc;*.c"] = ":/icons/cpp.png";
  m_DefaultIcons["*.h;*.hh;*.H;*.h++;*.hxx;*.hpp;*.hcc"] = ":/icons/hpp.png";
  m_DefaultIcons["CMake.in.config"] = ":/icons/cmake-config.png";
  m_DefaultIcons["CMakeLists.txt;*.cmake"] = ":/icons/cmakelists.png";
  m_DefaultIcons["*.f;*.F;*.for;*.FOR;*.f90;*.F90;*.fpp;*.FPP;*.f95;*.F95"] = ":/icons/fortran.png";
  m_DefaultIcons["*.ui"] = ":/icons/qt-ui.png";
  m_DefaultIcons["wareshub.json"] = ":/icons/wareshub.png";

  m_UserIcons = openfluid::ui::waresdev::WareSrcFiletypeManager::instance()
      ->getIconsByFileExtensionList();

  setRootPath(m_DirPath);
}


// =====================================================================
// =====================================================================


QVariant WareSrcExplorerModel::data(const QModelIndex& Index, int Role) const
{
  if (Index.isValid() && (Role == Qt::ForegroundRole
      || Role == QFileSystemModel::FileIconRole || Role == Qt::FontRole))
  {
    QFileInfo Info = fileInfo(Index);

    if (Info.isDir())
    {
      if (Info.fileName().startsWith("_build") && Role == Qt::ForegroundRole)
        return QVariant(QColor("#A3A3A3"));
      else if (Info.dir() == m_DirPath
          && Role == QFileSystemModel::FileIconRole)
        return QIcon(":/icons/waredir.png");
    }
    else if (Info.isFile())
    {
      if (Role == QFileSystemModel::FileIconRole)
      {
        for (QMap<QString, QString>::const_iterator it = m_UserIcons.begin();
            it != m_UserIcons.end(); ++it)
        {
          if (QDir::match(it.key(), Info.fileName()))
            return QIcon(it.value());
        }

        for (QMap<QString, QString>::const_iterator it = m_DefaultIcons.begin();
            it != m_DefaultIcons.end(); ++it)
        {
          if (QDir::match(it.key(), Info.fileName()))
            return QIcon(it.value());
        }

        return QIcon(":/icons/notype.png");
      }

      if (Role == Qt::FontRole)
      {
        openfluid::waresdev::WareSrcManager::PathInfo PInfo =
            openfluid::waresdev::WareSrcManager::instance()->getPathInfo(
                Info.absoluteFilePath());

        if (openfluid::waresdev::WareSrcContainer(PInfo.m_AbsolutePathOfWare,
                                                  PInfo.m_WareType,
                                                  PInfo.m_WareName)
            .getMainCppFile()
            == Info.absoluteFilePath())
        {
          QFont Font;
          Font.setBold(true);
          return Font;
        }
      }

    }
  }

  return QFileSystemModel::data(Index, Role);
}


// =====================================================================
// =====================================================================
