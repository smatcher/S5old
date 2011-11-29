#include "tools/widgets/resourceswidget.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "core/graphics/mesh.h"
#include "core/graphics/texture.h"
#include "core/graphics/material.h"
#include "core/graphics/shader.h"
#include "core/resources/sample.h"

#include "core/resources/resourcemanager.h"

#include "core/log/log.h"

QString ResourcesWidget::tabNames[ResourcesWidget::NB_TABS] =
{
	"materials",
	"meshes",
	"shaders",
	"sounds",
	"textures"
};


ResourcesWidget::ResourcesWidget() : QWidget()
{
	m_tab = new QTabWidget();
	m_current_tab = 0;

	m_listmodels[MATERIALS] = new ResourceManagerModel<MaterialData, Material>();
	m_listmodels[MESHES]    = new ResourceManagerModel<MeshData, Mesh>();
	m_listmodels[SHADERS]   = new ResourceManagerModel<ShaderProgramData, ShaderProgram>();
	m_listmodels[SOUNDS]    = new ResourceManagerModel<SampleData, Sample>();
	m_listmodels[TEXTURES]  = new ResourceManagerModel<TextureData, Texture>();

	for(int i=0 ; i<NB_TABS ; i++) {
		m_frames[i] = new QFrame();
		QHBoxLayout* layout = new QHBoxLayout();
		m_frames[i]->setLayout(layout);
		m_listviews[i] = new QListView();
		m_listviews[i]->setModel(m_listmodels[i]);
		layout->addWidget(m_listviews[i]);
		m_tab->addTab(m_frames[i],tabNames[i]);
	}

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_tab);
	setLayout(layout);

	connect(m_tab,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
	connect(m_listviews[0]->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(currentResourceChanged(QItemSelection,QItemSelection)));
}

ResourcesWidget::~ResourcesWidget()
{
}

void ResourcesWidget::currentTabChanged(int index)
{
	disconnect(m_listviews[m_current_tab]->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(currentResourceChanged(QItemSelection,QItemSelection)));
	m_current_tab = index;
	connect(m_listviews[m_current_tab]->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(currentResourceChanged(QItemSelection,QItemSelection)));
}

void ResourcesWidget::currentResourceChanged ( const QItemSelection & current, const QItemSelection & previous )
{
	QModelIndexList previous_indexes = previous.indexes();
	QModelIndexList current_indexes = current.indexes();

	if(!previous_indexes.isEmpty())
	{
		if(previous_indexes.first().isValid())
		{
			ResourcesWidget* old_widget = (ResourcesWidget*)previous_indexes.first().data(Qt::UserRole).value<void*>();
			if(old_widget)
			{
				m_frames[m_current_tab]->layout()->removeWidget(old_widget);
				delete old_widget;
			}
		}
	}

	if(!current_indexes.isEmpty())
	{
		if(current_indexes.first().isValid())
		{
			ResourcesWidget* new_widget = (ResourcesWidget*)current_indexes.first().data(Qt::UserRole).value<void*>();
			if(new_widget)
			{
				m_frames[m_current_tab]->layout()->addWidget(new_widget);
			}
		}
	}
}
