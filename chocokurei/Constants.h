#pragma once

inline const Size SceneSize{ 168, 168 };
inline const int DefaultSceneScale = 3;
inline const int ChocolateTypesCount = 12;
inline const int ConditionCount = 8;
inline const int SceneScaleMax = 8;
inline const int SceneScaleMin = 8;
inline const String TitleText = U"プロ生ちゃんチョコくれい";
inline const String WindowTitleText = U"chocokurei2023";
inline const String VersionText = U"Ver.2023021201";

inline const Array<String> TitleGuidanceText = {
	U"マウスクリックでスタートだよ",
	U"↑・↓キーで画面の大きさを変えられるよ",
};

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
	U"を食べてみて",
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
	U"ずいぶん悩むんだね…",
};

const Array<Array<String>> EndingText = {
	{
		U"「どう？ この日のために、たくさん用意したんだよ！」",
		U"プロ生ちゃんから、きれいに包装されたチョコの詰め合わせを受け取った。",
		U"どうやらプロ生ちゃん手作りのチョコらしい。甘くてほろ苦くて、とてもおいしいと感じる。",
		U"「遠慮しないで、どんどん食べて！ おいしい？」",
		U"プロ生ちゃんに促されるが、うまく手が動かない。プロ生ちゃんが、笑顔でこちらを見ている。",
		U"これは現実ではないのか？ 視界が白い霧に包まれていく中で、この夢が少しでも長く続いてくれと願うのだった。\n- Good End -",
	},
	{
		U"イベント会場でプロ生ちゃんの機嫌を損ねてしまった俺は、スタッフに腕を掴まれ会場の外へつまみ出された。",
		U"プロ生ちゃんの冷たい視線が目に焼き付いている。なぜ、こんなことになってしまったのか。",
		U"俺はただ、プロ生ちゃんからチョコをたくさんもらいたかっただけなのに……",
		U"しかし、代償は大きかった。\n――――今後、イベントへの出入り禁止を言い渡された俺は、",
		U"Twitterアカウントを削除し、その後一切プロ生ちゃんと関わることはなかった。\n- Bad End -",
	},
};
