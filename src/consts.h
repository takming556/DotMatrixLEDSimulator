using std::wstring;

constexpr int RES_SCR_X = 1280;   //画面解像度 幅[px]
constexpr int RES_SCR_Y = 720;    //画面解像度 高[px]

constexpr int RES_LED_X = 128;    //LEDの個数 幅[個]
constexpr int RES_LED_Y = 72;     //LEDの個数 高[個]

//Scene
constexpr int INITIAL = 0;        //初期画面
constexpr int SCREENING = 1;      //動画再生中の画面

//よく使いそうな色を定数であらかじめ定義しておく
const unsigned int White = GetColor(255, 255, 255);
const unsigned int Red = GetColor(255, 0, 0);
const unsigned int Blue = GetColor(0, 0, 255);
const unsigned int Green = GetColor(0, 255, 0);
const unsigned int Black = GetColor(0, 0, 0);
const unsigned int Yellow = GetColor(255, 255, 0);
const unsigned int Cyan = GetColor(0, 255, 255);
const unsigned int Mazenta = GetColor(255, 0, 255);
const unsigned int Orange = GetColor(255, 127, 0);
const unsigned int Purple = GetColor(127, 0, 255);
const unsigned int Pink = GetColor(255, 0, 127);
const unsigned int Sky = GetColor(0, 127, 255);

const wstring KeyGuideForScreening =
L"[F1].....操作説明表示<入|切>"
L"\n[F2].....再生待機列表示<入|切>"
L"\n[F3].....状態表示<入|切>"
L"\n[F11]....全画面表示<入|切>"
L"\n[W]......色深度+"
L"\n[S]......色深度-"
L"\n[A]......ガンマ補正+"
L"\n[D]......ガンマ補正-"
L"\n[Space]..再生/一時停止"
L"\n[←].....5秒戻る"
L"\n[→].....5秒進む"
L"\n[Z]......前の動画へ"
L"\n[C]......次の動画へ"
L"\n[B]......この動画の先頭へ"
L"\n[↑].....音量+"
L"\n[↓].....音量-"
L"\n[R]......単体循環<入|切>"
L"\n[T]......全体循環<入|切>"
L"\n[F]......乱順再生<入|切>"
L"\n[M]......単色表示<入|切>"
L"\n[N]......反色表示<入|切>"
L"\n[O]......ファイルを再生待機列に追加する"
L"\n[Esc]....上映を終了する";

const wstring KeyGuideForInitial =
L"[O]........ファイルを再生待機列に追加する"
L"\n[Delete]...再生待機列を破棄する"
L"\n[Enter]....上映を開始する"
L"\n[F1].......操作説明表示<入|切>"
L"\n[F2].......再生待機列表示<入|切>"
L"\n[F3].......状態表示<入|切>"
L"\n[F11]......全画面表示<入|切>"
L"\n[F]........乱順再生<入|切>"
L"\n[Esc]......プログラムを終了する";