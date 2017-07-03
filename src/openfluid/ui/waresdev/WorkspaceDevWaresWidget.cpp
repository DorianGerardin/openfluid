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
  @file WorkspaceDevWaresWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/ui/waresdev/WorkspaceDevWaresWidget.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevGitWidget.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevActionsWidget.hpp>

#include "ui_WorkspaceDevWaresWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


const WorkspaceDevDashboardTypes::ActionsByRows WorkspaceDevWaresWidget::Actions =
{
  {{"configure","Configure"},{"build","Build"}},
  {{"wareshub","wareshub.json"},{"readme","README"},{"license","LICENSE"}},
  {{"purge","Purge"}}
};


// =====================================================================
// =====================================================================


WorkspaceDevWaresWidget::WorkspaceDevWaresWidget(QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WorkspaceDevWaresWidget)
{
  ui->setupUi(this);

  ui->WaresTableWidget->horizontalHeader()->setStretchLastSection(true);

  connect(ui->SelectAllActionLabel, SIGNAL(clicked()), this, SLOT(selectAll()));
  connect(ui->SelectNoneActionLabel, SIGNAL(clicked()), this, SLOT(selectNone()));

  connect(ui->WaresTableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(notifySelectionChanged()));
}


// =====================================================================
// =====================================================================


WorkspaceDevWaresWidget::~WorkspaceDevWaresWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WorkspaceDevWaresWidget::selectAll()
{
  for (unsigned int i = 0; i < m_WaresCount; i++)
    ui->WaresTableWidget->item(i,0)->setCheckState(Qt::Checked);
}


// =====================================================================
// =====================================================================


void WorkspaceDevWaresWidget::selectNone()
{
  for (unsigned int i =0; i < m_WaresCount; i++)
    ui->WaresTableWidget->item(i,0)->setCheckState(Qt::Unchecked);
}



// =====================================================================
// =====================================================================


void WorkspaceDevWaresWidget::notifySelectionChanged()
{
  emit selectionChanged();
}


// =====================================================================
// =====================================================================


void WorkspaceDevWaresWidget::addWare(const WorkspaceDevDashboardTypes::WareGitInfos& Infos)
{
  m_WaresCount++;

  ui->WaresTableWidget->setRowCount(m_WaresCount);

  QTableWidgetItem *TableItem = nullptr;

  TableItem = new QTableWidgetItem(Infos.ID);
  TableItem->setData(Qt::UserRole,Infos.Path);
  TableItem->data(Qt::CheckStateRole);
  TableItem->setCheckState(Qt::Unchecked);
  ui->WaresTableWidget->setItem(m_WaresCount-1, 0, TableItem);

  ui->WaresTableWidget->setCellWidget(m_WaresCount-1, 1, new WorkspaceDevGitWidget(Infos));

  WorkspaceDevActionsWidget* ActionsWidget = new WorkspaceDevActionsWidget(Actions);
  m_ActionsWidgetsMap[Infos.ID] = ActionsWidget;
  ui->WaresTableWidget->setCellWidget(m_WaresCount-1, 2, ActionsWidget);

  ui->WaresTableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
  ui->WaresTableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
  ui->WaresTableWidget->horizontalHeader()->stretchLastSection();
}


// =====================================================================
// =====================================================================


WorkspaceDevDashboardTypes::WaresSelection WorkspaceDevWaresWidget::getSelection()
{
  WorkspaceDevDashboardTypes::WaresSelection Selection;

  for (unsigned int i =0; i < m_WaresCount; i++)
  {
    QTableWidgetItem *Item = ui->WaresTableWidget->item(i,0);
    if (Item->checkState() == Qt::Checked)
    {
      WorkspaceDevDashboardTypes::WareProcessInfos PInfos;
      PInfos.ID = Item->text();
      PInfos.Path = Item->data(Qt::UserRole).toString();
      Selection.push_back(PInfos);
    }
  }

  return Selection;
}


// =====================================================================
// =====================================================================


void WorkspaceDevWaresWidget::setState(const QString& ID, const QString& ActionName, bool IsOK)
{
  auto it = m_ActionsWidgetsMap.find(ID);

  if (it != m_ActionsWidgetsMap.end())
    (*it).second->setState(ActionName,IsOK);
}


} } } // namespaces
