using std::wstring;

constexpr int RES_SCR_X = 1280;   //��ʉ𑜓x ��[px]
constexpr int RES_SCR_Y = 720;    //��ʉ𑜓x ��[px]

constexpr int RES_LED_X = 128;    //LED�̌� ��[��]
constexpr int RES_LED_Y = 72;     //LED�̌� ��[��]

//Scene
constexpr int INITIAL = 0;        //�������
constexpr int SCREENING = 1;      //����Đ����̉��

//�悭�g�������ȐF��萔�ł��炩���ߒ�`���Ă���
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
L"[F1].....��������\��<��|��>"
L"\n[F2].....�Đ��ҋ@��\��<��|��>"
L"\n[F3].....��ԕ\��<��|��>"
L"\n[F11]....�S��ʕ\��<��|��>"
L"\n[W]......�F�[�x+"
L"\n[S]......�F�[�x-"
L"\n[A]......�K���}�␳+"
L"\n[D]......�K���}�␳-"
L"\n[Space]..�Đ�/�ꎞ��~"
L"\n[��].....5�b�߂�"
L"\n[��].....5�b�i��"
L"\n[Z]......�O�̓����"
L"\n[C]......���̓����"
L"\n[B]......���̓���̐擪��"
L"\n[��].....����+"
L"\n[��].....����-"
L"\n[R]......�P�̏z��<��|��>"
L"\n[T]......�S�̏z��<��|��>"
L"\n[F]......�����Đ�<��|��>"
L"\n[M]......�P�F�\��<��|��>"
L"\n[N]......���F�\��<��|��>"
L"\n[O]......�t�@�C�����Đ��ҋ@��ɒǉ�����"
L"\n[Esc]....��f���I������";

const wstring KeyGuideForInitial =
L"[O]........�t�@�C�����Đ��ҋ@��ɒǉ�����"
L"\n[Delete]...�Đ��ҋ@���j������"
L"\n[Enter]....��f���J�n����"
L"\n[F1].......��������\��<��|��>"
L"\n[F2].......�Đ��ҋ@��\��<��|��>"
L"\n[F3].......��ԕ\��<��|��>"
L"\n[F11]......�S��ʕ\��<��|��>"
L"\n[F]........�����Đ�<��|��>"
L"\n[Esc]......�v���O�������I������";