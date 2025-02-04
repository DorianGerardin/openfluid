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
  @file ExamplesManager.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <iostream>

#include <openfluid/base/ExamplesManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


// TOIMPL add unit tests


namespace openfluid { namespace base {


std::string ExamplesManager::buildRessourcesPath(const std::string& Path)
{
  return (Path.empty() ? openfluid::base::Environment::getProvidedExamplesDir() : Path);
}


// =====================================================================
// =====================================================================


std::string ExamplesManager::buildInstallPath(const std::string& Path)
{
  return (Path.empty() ? openfluid::base::Environment::getUserExamplesDir() : Path);
}


// =====================================================================
// =====================================================================


bool ExamplesManager::installDirectory(const std::string& FromPath, const std::string& ToPath,
                                       const std::string& DirName, const bool Force)
{
  auto InstallTargetPathObj = openfluid::tools::FilesystemPath({ToPath,DirName});

  if (!InstallTargetPathObj.isDirectory() || Force)
  {
    InstallTargetPathObj.makeDirectory();

    std::cout << "[Test exemples] " << DirName << " copy directory" << std::endl;

    return openfluid::tools::Filesystem::copyDirectory(openfluid::tools::Filesystem::joinPath({FromPath,DirName}),
                                                       ToPath,true,true);
  }

  return false;
}

    
// =====================================================================
// =====================================================================


bool ExamplesManager::installProject(const std::string& ProjectDir,
                                     const std::string& ResourcesPath, const std::string& InstallPath,
                                     const bool Force)
{
  std::string FromPath = openfluid::tools::Filesystem::joinPath({buildRessourcesPath(ResourcesPath),
                                                                 openfluid::config::PROJECTS_PATH});
  std::cout << "Test exemples " << "From Path(project) : " << FromPath << std::endl;
  if (!openfluid::tools::FilesystemPath({FromPath,ProjectDir}).isDirectory())
  {
    // silent since called for every ware import
    std::cout << "[Test exemples] " << ProjectDir << " (project) is not a directory" << std::endl;
    return false;
  }
  std::string ToPath = openfluid::tools::Filesystem::joinPath({buildInstallPath(InstallPath),
                                                               openfluid::config::PROJECTS_PATH});
  std::cout << "[Test exemples] " << "To Path(project) : " << ToPath << std::endl;
  std::cout << "-- Installing project " << ProjectDir << " from " << FromPath << " to " << ToPath << std::endl;
  bool HasInstalledDirectory = installDirectory(FromPath, ToPath, ProjectDir, Force);
  std::cout << "[Test exemples] " << "Installed directory(project) : " << HasInstalledDirectory << std::endl;
  return HasInstalledDirectory;
}


// =====================================================================
// =====================================================================


bool ExamplesManager::installSimulator(const std::string& SimulatorDir,
                                       const std::string& ResourcesPath, const std::string& InstallPath,
                                       const bool Force)
{
  std::string FromPath = openfluid::tools::Filesystem::joinPath({buildRessourcesPath(ResourcesPath),
                                                                 openfluid::config::WARESDEV_PATH,
                                                                 openfluid::config::SIMULATORS_PATH});
  std::cout << "[Test exemples] " << "From Path(simulator) : " << FromPath << std::endl;
  if (!openfluid::tools::FilesystemPath({FromPath,SimulatorDir}).isDirectory())
  {
    // silent since called for every ware import
    std::cout << "[Test exemples] " << SimulatorDir << " (simulator) is not a directory" << std::endl;
    return false;
  }
  std::string ToPath = openfluid::tools::Filesystem::joinPath({buildInstallPath(InstallPath),
                                                               openfluid::config::WARESDEV_PATH,
                                                               openfluid::config::SIMULATORS_PATH});
  std::cout << "[Test exemples] " << "To Path(simulator) : " << ToPath << std::endl;
  std::cout << "-- Installing simulator " << SimulatorDir << " from " << FromPath << " to " << ToPath << std::endl;
  bool HasInstalledDirectory = installDirectory(FromPath,ToPath,SimulatorDir,Force);
  std::cout << "[Test exemples] " << "Installed directory(simulator) : " << HasInstalledDirectory << std::endl;
  return HasInstalledDirectory;
}


// =====================================================================
// =====================================================================


bool ExamplesManager::installAllProjects(const std::string& ResourcesPath, const std::string& InstallPath, 
                                         const bool Force)
{
  auto ResPath = buildRessourcesPath(ResourcesPath);
  auto InstPath = buildInstallPath(InstallPath);
  std::cout << "[Test exemples] " << "Res Path (projects) : " << ResPath << std::endl;
  std::cout << "[Test exemples] " << "Install Path (projects) : " << InstPath << std::endl;

  std::string ProjectsPath = openfluid::tools::Filesystem::joinPath({ResPath,openfluid::config::PROJECTS_PATH});
  std::cout << "[Test exemples] " << "Projects Path : " << ProjectsPath << std::endl;

  if (openfluid::tools::FilesystemPath(ProjectsPath).isDirectory())
  {
    std::cout << "[Test exemples] " << "Projects Path is directory" << std::endl;
    std::vector<std::string> FoundProjects = openfluid::tools::Filesystem::findDirectories(ProjectsPath);
    std::cout << "[Test exemples] " << "Found : " << FoundProjects.size() << " projects" << std::endl;

    bool AllIsOK = true;
    for (const auto& Prj : FoundProjects)
    {
      AllIsOK += installProject(Prj,ResPath,InstPath,Force);
    }
    return AllIsOK;
  }
  std::cout << "[Test exemples] " << "Projects Path is not a directory" << std::endl;
  return false;
}


// =====================================================================
// =====================================================================


bool ExamplesManager::installAllSimulators(const std::string& ResourcesPath, const std::string& InstallPath,
                                           const bool Force)
{
  auto ResPath = buildRessourcesPath(ResourcesPath);
  auto InstPath = buildInstallPath(InstallPath);
  std::cout << "[Test exemples] " << "Res Path (simulators) : " << ResPath << std::endl;
  std::cout << "[Test exemples] " << "Install Path (simulators) : " << InstPath << std::endl;

  std::string SimulatorsPath = openfluid::tools::Filesystem::joinPath({ResPath,
                                                                       openfluid::config::WARESDEV_PATH,
                                                                       openfluid::config::SIMULATORS_PATH});
  
  std::cout << "[Test exemples] " << "Simulators Path : " << SimulatorsPath << std::endl;
  if (openfluid::tools::FilesystemPath(SimulatorsPath).isDirectory())
  {
    std::cout << "[Test exemples] " << "Simulators Path is directory" << std::endl;
    std::vector<std::string> FoundSimulators = openfluid::tools::Filesystem::findDirectories(SimulatorsPath);
    std::cout << "[Test exemples] " << "Found : " << FoundSimulators.size() << " simulators" << std::endl;

    bool AllIsOK = true;
    for (const auto& Sim : FoundSimulators)
    {
      AllIsOK += installSimulator(Sim,ResPath,InstPath,Force);
    }
    return AllIsOK;
  }
  std::cout << "[Test exemples] " << "Simulators Path is not a directory" << std::endl;
  return false;
}


// =====================================================================
// =====================================================================


bool ExamplesManager::installAll(const std::string& ResourcesPath, const std::string& InstallPath, const bool Force)
{
  return (installAllProjects(ResourcesPath,InstallPath,Force) && 
          installAllSimulators(ResourcesPath,InstallPath,Force));
}


} } // namespaces
