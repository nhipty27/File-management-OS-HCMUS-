#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "Header.h"
using namespace std;
void startMenu() {
		cout << "-----------------Moi chon cac yeu cau-----------------" << endl;
		cout << "0: thoat" << endl;
		cout << "1: Tao, dinh dang volume MyFS.dat" << endl;
		cout << "2: Thiet lap, doi, kiem tra mat khau truy xuat MyFS" << endl;
		cout << "3: liet ke danh sach cac tap tin trong MyFS" << endl;
		cout << "4: dat, doi mat khau truy xuat cho 1 tap tin trong MyFS" << endl;
		cout << "5: chep 1 tap tin tu ben ngoai vao MyFS" << endl;
		cout << "6: chep 1 tap tin tu MyFS ra ngoai" << endl;
		cout << "7: xoa 1 tap tin trong myFS" << endl;
}
int main()
{
	string volName = "a.dat", password, confirmPass;
	uint32_t sizeVol;
	//while (1) {
	//	int n;
	//	cout << "chon cac yeu cau" << endl;
	//	cout << "0: thoat" << endl;
	//	cout << "1: Tao, dinh dang volume MyFS.dat" << endl;
	//	cout << "2: Thiet lap, doi, kiem tra mat khau truy xuat MyFS" << endl;
	//	cout << "3: liet ke danh sach cac tap tin trong MyFS" << endl;
	//	cout << "4: dat, doi mat khau truy xuat cho 1 tap tin trong MyFS" << endl;
	//	cout << "5: chep 1 tap tin tu ben ngoai vao MyFS" << endl;
	//	cout << "6: chep 1 tap tin tu MyFS ra ngoai" << endl;
	//	cout << "7: xoa 1 tap tin trong myFS" << endl;
	//	cin >> n;
	//	if (n == 0)
	//		break;
	//	/*switch (n)
	//	{
	//	case 1:
	//		cout << "-----------------Tao volume----------------" << endl;
	//		cout << "1:tu nhap kich thuoc volume" << endl;
	//		cout << "2:chon kich thuoc volume de xuat" << endl;
	//		while (1) {
	//			cin >> n;
	//			if (n == 1) {
	//				cout << "kich thuoc volume toi thieu là 100MB toi da la 2048 MB" << endl;
	//				cout << "nhap kich thuoc: ";
	//				while (1) {
	//					cin >> sizeVol;
	//					if (sizeVol <= 2048)
	//						break;
	//				}
	//				break;
	//			}
	//			else if (n == 2) {
	//				cout << "chon cac kich thuoc volume duoc de xuat duoi day" << endl;
	//				cout << "1: kich thuoc =128MB" << endl;
	//				cout << "2: kich thuoc =256MB" << endl;
	//				cout << "3: kich thuoc =512MB" << endl;
	//				cout << "4: kich thuoc =1024MB" << endl;
	//				cout << "5: kich thuoc =2048MB" << endl;
	//				cin >> n;
	//				if (n == 1) {
	//					sizeVol = 128;
	//					break;
	//				}
	//				else if (n == 2) {
	//					sizeVol = 256;
	//					break;
	//				}
	//				else if (n == 3) {
	//					sizeVol = 512;
	//					break;
	//				}
	//				else if (n == 4) {
	//					sizeVol = 1024;
	//					break;
	//				}
	//				else if (n == 5) {
	//					sizeVol = 2048;
	//					break;
	//				}
	//				else {
	//					cout << "loi! hay thu lai sau" << endl << endl;
	//				}
	//			}
	//		}
	//		cin.ignore();
	//		cout << "Nhap ten volume can tao (bo qua => ten volume: MyFS.dat)" ;
	//		getline(cin, volName);
	//		if (volName.empty())
	//			volName = "MyFS.dat";
	//		while (1) {
	//			cout << "Nhap mat khau: " << endl;
	//			getline(cin, password);
	//			cout << "Nhap lai mat khau: " << endl;
	//			getline(cin, confirmPass);
	//			if (password != confirmPass) {
	//				cout << "Mat khau khong chinh xac. Moi nhap lai!" << endl;
	//			}
	//			else break;
	//		}
	//		initVolume(volName, password, sizeVol);
	//		
	//		break;
	//	case 2:
	//		cout << "-----------------Thiet lap, doi, kiem tra mat khau----------------" << endl;
	//		cout << "1:Thiet lap mat khau" << endl;
	//		cout << "2:Doi mat khau" << endl;
	//		cout << "3:Kiem tra mat khau" << endl;
	//		cin >> n;
	//		PassAction(volName, n);
	//		break;
	//	case 3:
	//		FatTable a(128);
	//		a = readFatTable(volName);
	//		BootSector* b = new BootSector(128,"abc");
	//		b->convertBootsector(volName);
	//		Entry A("abcdefghijk.txt", "123456789");
	//		writeEntry(volName, b, a, A);
	//		break;
	//	case 4:
	//		cout << "dat/doi mk";
	//		break;
	//	case 5:
	//		cout << "import";
	//		break;
	//	case 6:
	//		cout << "export";
	//		break;
	//	case 7:
	//		cout << "delete";
	//		break;
	//	default:
	//		break;
	//	}*/

	//}
	initVolume("a.dat", "123", 128);
	FatTable a(128);
	a = readFatTable(volName);
	BootSector* b = new BootSector(128, "abc");
	b->convertBootsector(volName);
	Entry A("abcdefghijkasdnaskfocnskncosdjwndksa.txt", "123456789");
	writeEntry(volName, b, a, A);
	Entry B("ab.txt", "123456789");
	writeEntry(volName, b, a, B); 
	Entry C("abcdefghijk.txt", "123456789");
	writeEntry(volName, b, a, C);
	vector<Entry>list;
	list = readEntry("a.dat", b);
	importFile("a.dat", b, a, "D:\\a", "abc");
	return 0;
}
