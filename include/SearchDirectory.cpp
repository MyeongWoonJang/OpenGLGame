#include <iostream> //I/O��Ʈ�� ���
#include <io.h> //���� ����ü ���
#include <string>//��Ʈ�� ��ü ��� ���
#include <list>//����Ʈ �ڷ��� ���

using namespace std;

void searchingDir( string path );
int isFileOrDir( _finddata_t fd );

int main() {
	string path = "resources/object";
	searchingDir( path );

}

void searchingDir( string path )
{
	int checkDirFile = 0;
	string dirPath = path + "\\*.*";
	struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
	intptr_t handle;
	list<_finddata_t> fdlist;//���丮, ���� ���� ��ü ����Ʈ

	if ( ( handle = _findfirst( dirPath.c_str(), &fd ) ) == -1L ) //fd ����ü �ʱ�ȭ.
	{
		//�����̳� ���丮�� ���� ���.
		cout << "No file in directory!" << endl;
		return;
	}

	do //���� Ž�� �ݺ� ����
	{
		checkDirFile = isFileOrDir( fd );//���� ��ü ���� Ȯ��(���� or ���丮)
		if ( checkDirFile == 0 && fd.name[ 0 ] != '.' ) {
			//���丮�� ���� ����
			cout << "Dir  : " << path << "\\" << fd.name << endl;
			searchingDir( path + "\\" + fd.name );//����� ȣ��
		}
		else if ( checkDirFile == 1 && fd.name[ 0 ] != '.' ) {
			//������ ���� ����
			cout << '"' << path << "/" << fd.name << "\",\n";
			fdlist.push_back( fd );
		}

	} while ( _findnext( handle, &fd ) == 0 );
	_findclose( handle );
}

int isFileOrDir( _finddata_t fd )
//�������� ���丮���� �Ǻ�
{

	if ( fd.attrib & _A_SUBDIR )
		return 0; // ���丮�� 0 ��ȯ
	else
		return 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

}