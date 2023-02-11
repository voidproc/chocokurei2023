#pragma once

inline const Size SceneSize{ 168, 168 };
inline const int DefaultSceneScale = 4;
inline const int ChocolateTypesCount = 12;
inline const int ConditionCount = 8;

inline const Array<String> ConditionText = {
	U"すきなチョコ",
	U"ハート型のチョコ",
	U"まるいチョコ",
	U"四角いチョコ",
	U"茶色のチョコ",
	U"ホワイトチョコ",
	U"一番多いの",
	U"一番少ないの",
};

inline const Array<String> ConditionTextEnd = {
	U"を取っていいよ",
	U"を食べていいよ",
	U"をあげる",
	U"をどうぞ",
	U"を取ってね",
	U"を食べてね",
};

const Array<String> SuccessText = {
	U"おいしい？",
	U"どう？ おいしい？",
	U"はい、どうぞ！",
};

const Array<String> FailedText = {
	U"うーん、それじゃないんだけど？",
	U"ちょっと、それじゃないよー！",
	U"それ私が食べたかったのに～",
	U"私が言ったこと覚えてる？！",
	U"あっ…　まあ、いいけどね",
};

const Array<String> TimeUpText = {
	U"時間切れ～　またこんどね！",
	U"ずいぶん悩むのね…",
};

