#include "core/graphics/ubershader.h"

UberShaderDefine::Type UberShaderDefine::fromStr(const QString& name)
{
	Type ret = UNKNOWN;
	for(int i=0 ; i<NB_DEFINES ; i++) {
		if(Names[Type(i)] == name) {
			ret = Type(i);
			break;
		}
	}
	return ret;
}

QString UberShaderDefine::toString(int i)
{
	return toString(Type(i));
}

QString UberShaderDefine::toString(Type type)
{
	if(type >= 0 && type <= UNKNOWN)
		return Names[type];
	else
		return Names[UNKNOWN];
}

UberShaderTextureType::Type UberShaderTextureType::fromStr(const QString& name)
{
	Type ret = UNKNOWN;
	for(int i=0 ; i<NB_TEXTURES ; i++) {
		if(Names[Type(i)] == name) {
			ret = Type(i);
			break;
		}
	}
	return ret;
}

QString UberShaderTextureType::toString(int i)
{
	return toString(Type(i));
}

QString UberShaderTextureType::toString(Type type)
{
	if(type >= 0 && type <= UNKNOWN)
		return Names[type];
	else
		return Names[UNKNOWN];
}

