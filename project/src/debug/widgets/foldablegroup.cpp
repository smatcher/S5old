#include "include/debug/widgets/foldablegroup.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

FoldableGroup::FoldableGroup(const QString &name)
{
	m_label = new QLabel(name);
	m_button = new QPushButton(tr("+"));
	m_group = NULL;
	m_layout = new QGridLayout();

	m_layout->addWidget(m_button);
	m_layout->addWidget(m_label);

	setLayout(m_layout);
}

FoldableGroup::~FoldableGroup()
{

}

void FoldableGroup::setGroup(QWidget* group)
{
	if(m_group != NULL)
	{
		m_layout->removeWidget(m_group);
		delete m_group;
	}
	m_group = group;
	m_layout->addWidget(m_group);
}
