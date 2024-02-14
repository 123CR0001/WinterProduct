#include"MotionComponent.h"
#include"CharaBase.h"
#include"AnimationComponent.h"
#include"ObjectServer.h"
#include"ModeGame.h"
#include"ModeEffekseer.h"

#include"ApplicationGlobal.h"


std::unordered_map<std::string, std::function<void(const MOTION_DATA_ITEM&)>> MotionComponent::_commandFuncMap;

MotionComponent::MotionComponent(CharaBase* owner,int order)
	:Component(owner,order)
	, _chara(owner)
	, _motCnt(0) 
{
	_commandFuncMap["MOVE"] = [this](const MOTION_DATA_ITEM& item) {
		
		float rad = DegToRad(item.vector) + _chara->GetEulerAngle().y;

		Vector3D v(sin(rad), 0, cos(rad));
		v.Normalized();

		_chara->AddPos( v * item.vectorScale);

		_motCnt++; 
	};

	_commandFuncMap["LOOP"] = [this](const MOTION_DATA_ITEM& item) {_motCnt = 0; };

	//_commandFuncMap["CHANGE_MOTION"] = [this](const MOTION_DATA_ITEM& item) {
	//	_chara->GetAnimationComponent()->ChangeAnimation(item.ChangeMotion.c_str());
	//	_motCnt;
	//};

	_commandFuncMap["PLAY_SOUND"] = [this](const MOTION_DATA_ITEM& item) {//SEの再生
		gGlobal._sndServer.Get(item.soundPlayName)->Play();
		_motCnt++;
	};

	_commandFuncMap["PLAY_EFFECT_3DFRAME"] = [this](const MOTION_DATA_ITEM& item) {
		//エフェクトの再生
		void* mode = ModeServer::GetInstance()->Get(MODE_EFFEKSEER_NAME);
		if (mode) {

			// フレーム名からフレーム番号を取得する
			int FrameIndex = MV1SearchFrame(_chara->GetHandle(), item.effectPlay3DFrame.c_str());

			// フレームの現在のワールドでの状態を示す行列を取得する
			MATRIX FrameMatrix = MV1GetFrameLocalWorldMatrix(_chara->GetHandle(), FrameIndex);

			ModeEffekseer* effect = _chara->GetObjectServer()->GetGame()->GetModeEffekseer();

			Vector3D angle = _chara->GetEulerAngle();

			effect->Play(
				item.effectPlayName,
				Vector3D(FrameMatrix.m[3][0], FrameMatrix.m[3][1], FrameMatrix.m[3][2]),
				Vector3D(angle.x, angle.y + PI, angle.z)
			);
		}

		_motCnt++;
	};
		/*		{"ATTACK_ON",MotionCommand::ATTACK_ON},
		{"ATTACK_OFF",MotionCommand::ATTACK_OFF},
		{"PLAY_EFFECT",MotionCommand::PLAY_EFFECT},
		{"PLAY_EFFECT_3DFRAME",MotionCommand::PLAY_EFFECT_3DFRAME},
		{"PLAY_SOUND",MotionCommand::PLAY_SOUND},
		{"LOOP",MotionCommand::LOOP},
		{"CHANGE_MOTION",MotionCommand::CHANGE_MOTION},
		{"MOVE",MotionCommand::MOVE}*/
}

MotionComponent::~MotionComponent(){}

bool MotionComponent::Process() {

	auto motionData = gGlobal._charaMotionData;

	//このコンポーネントを保持するオーナーの名前
	std::string name = _chara->GetName();
	AnimationComponent* anim = _chara->GetAnimationComponent();
	std::string animName = anim->GetAnimationName();

	//CharaBaseが再生しているアニメーションの再生時間を取得		切り上げ
	int playTime = static_cast<int>(anim->GetPlayTime() + 0.999f);

	if (motionData.find(name) == motionData.end()) { return false; }
	if (motionData[name].find(animName) == motionData[name].end()) { return false; }

	//モーションデータで指定された再生時間と一緒か
	if (playTime == motionData[name][animName][_motCnt].playTime) {

		const auto& data = motionData[name][animName][_motCnt];

		//共通のコマンド処理か
		if (_commandFuncMap.find(data.command) != _commandFuncMap.end()) {
			(_commandFuncMap[data.command])(data);
			return true;
		}

		//キャラ固有のコマンド処理か
		if (_customCommandFuncMap.find(data.command) != _customCommandFuncMap.end()){
			(_customCommandFuncMap[data.command])(data);
			return true;
		}

	}

	return true;
}
