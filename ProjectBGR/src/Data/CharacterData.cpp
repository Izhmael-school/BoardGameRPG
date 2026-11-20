#include "CharacterData.h"
#include "UI/UIContext.h"

CharacterData::CharacterData() {
	equipList.resize(EquipPosition_Max);
	status = std::make_unique<CharacterStatus>();
	MakeContext();
}

CharacterData::~CharacterData() {
	if (context)
		context.reset();
}

void CharacterData::MakeContext() {
	context.reset();
	context = std::make_shared<UIContext>();

	context->Register("Level", &status->GetLevel());
	context->Register("CurrentHP", &status->GetLevel());
	context->Register("MaxHP", &status->GetLevel());
	context->Register("Atk", &status->GetLevel());
	context->Register("Def", &status->GetLevel());
	context->Register("Vit", &status->GetLevel());
	context->Register("Mag", &status->GetLevel());
	context->Register("Spd", &status->GetLevel());
	context->Register("Luk", &status->GetLevel());
	context->Register("Money", &money);
	context->Register("Name", &playerName);
}
