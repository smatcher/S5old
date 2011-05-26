#include "tools/widgets/soundemitterwidget.h"
#include "tools/widgets/propertywidget.h"
#include "core/properties/soundemitter.h"
#include "core/resources/sample.h"
#include <QtGui>

SoundEmitterWidget::SoundEmitterWidget(SoundEmitter* property) : PropertyWidget(property)
{
		m_sample = new QLineEdit(property->m_sample->name());
		m_sample->setDisabled(true);

		if(property->m_type == SoundEmitter::TYPE_DIRECTIONAL)
				m_type = new QLineEdit("TYPE_DIRECTIONAL");
		else if(property->m_type == SoundEmitter::TYPE_GLOBAL)
				m_type = new QLineEdit("TYPE_GLOBAL");
		else if(property->m_type == SoundEmitter::TYPE_OMNIDIRECTIONAL)
				m_type = new QLineEdit("TYPE_OMNIDIRECTIONAL");
		else
				m_type = new QLineEdit("Unknown");

		m_type->setDisabled(true);

		QVBoxLayout* layout = new QVBoxLayout();
		layout->setMargin(0);
		m_box->setLayout(layout);

		layout->addWidget(m_sample);
		layout->addWidget(m_type);
}

SoundEmitterWidget::~SoundEmitterWidget()
{
}

void SoundEmitterWidget::updateData()
{
		SoundEmitter* property = static_cast<SoundEmitter*>(m_property);
		m_sample->setText(property->m_sample->name());
		if(property->m_type == SoundEmitter::TYPE_DIRECTIONAL)
				m_type = new QLineEdit("TYPE_DIRECTIONAL");
		else if(property->m_type == SoundEmitter::TYPE_GLOBAL)
				m_type = new QLineEdit("TYPE_GLOBAL");
		else if(property->m_type == SoundEmitter::TYPE_OMNIDIRECTIONAL)
				m_type = new QLineEdit("TYPE_OMNIDIRECTIONAL");
		else
				m_type = new QLineEdit("Unknown");
}

