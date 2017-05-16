#include "DataTransfer.h"

const char* json = "{\"Update_Lock\",\"Board\",\"ScoreB\",\"ScoreW\"}";
struct MyHandler {
	bool Null() { cout << "Null()" << endl; return true; }
	bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
	bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
	bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
	bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
	bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
	bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
	bool RawNumber(const char* str, SizeType length, bool copy) {
		cout << "Number(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool String(const char* str, SizeType length, bool copy) {
		cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool StartObject() { cout << "StartObject()" << endl; return true; }
	bool Key(const char* str, SizeType length, bool copy) {
		cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
	bool StartArray() { cout << "StartArray()" << endl; return true; }
	bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};
Document d1;
char readBuffer[65536];
DataTransfer::DataTransfer()
{
	InitailizeJson();
}


DataTransfer::~DataTransfer()
{
}


//����ļ��Ƿ��Ѿ����������ȡ�ˣ���ֹ�Ի�û��ȡ���ļ�����д����
void DataTransfer::CheckRead(int type){
	FILE* fp = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "rb"); // �� Windows ƽ̨ʹ�� "r"
	
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
	d1.ParseStream(is);
	printf("%d\n", d1["Whether_Read"].GetBool());
	int i = 0;
	
	//���ļ�û�б���ȡ������ͣ10msȻ�����²���
	while (!d1["Whether_Read"].GetBool()){
		i++;
		if (i % 50 == 0){
			printf("Type:%d The SendToUI.json has not been read by the UI# ",type);
			printf("Already pause %ds\n",i/10);
		}
		Sleep(100);		//��ͣ10ms
		
		reopen(1, fp);
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		d1.ParseStream(is);
	}
	fclose(fp);
}


//����Board��������
void DataTransfer::UpdateBoard(int b[8][8]){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			transfer_board[i][j] = b[i][j];
		}
	}
}
//����ѡ�����壬��̨���ºú󷵻ظ�UI��ʾ
bool DataTransfer::SendToUI(int b[8][8]){
	
	CheckRead(2);

	UpdateBoard(b);

	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // �� Windows ƽ̨ʹ�� "w"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	
	//������д��json�ļ��й�
	writer.StartObject();               // Between StartObject()/EndObject(), 
	writer.Key("Whether_Write");
	writer.Bool(true);
	writer.Key("Whether_Read");
	writer.Bool(false);
	writer.Key("ScoreB");
	writer.Uint(ScoreB);
	writer.Key("ScoreW");
	writer.Uint(ScoreW);
	writer.Key("Board");
	writer.StartArray();                // Between StartArray()/EndArray(),
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8;j++)
			writer.Uint(transfer_board[i][j]);                 // all values are elements of the array.
	writer.EndArray();

	writer.EndObject();
	fclose(fp1);
	return 0;
}

bool DataTransfer::SendToUI(int x,int y,int b[8][8]){


	CheckRead(1);
	UpdateBoard(b);
	
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // �� Windows ƽ̨ʹ�� "w"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	//������д��json�ļ���
	writer.StartObject();               // Between StartObject()/EndObject(), 
	writer.Key("PositionX");
	writer.Uint(x);
	writer.Key("PositionY");
	writer.Uint(y);
	writer.Key("TimeWait");
	writer.Uint(TimeWait);
	writer.Key("Whether_Write");
	writer.Bool(true);
	writer.Key("Whether_Read");
	writer.Bool(false);
	writer.Key("ScoreB");
	writer.Uint(ScoreB);
	writer.Key("ScoreW");
	writer.Uint(ScoreW);
	writer.Key("Board");
	writer.StartArray();                // Between StartArray()/EndArray(),
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			writer.Uint(transfer_board[i][j]);                 // all values are elements of the array.
	writer.EndArray();

	writer.EndObject();
	fclose(fp1);
	return 0;
}


void DataTransfer::reopen(int type, FILE *fp){
	
	fclose(fp);
	//���´�sendUI�ļ�
	if (type == 1){
		fclose(fp);
		fp = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "rb");
	}
	else{ //��fromUI�ļ�
		fclose(fp);
		fp = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "rb"); // �� Windows ƽ̨ʹ�� "r"
	}

}

_Coordinate DataTransfer::GetFromUI(){
	FILE* fp = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "rb"); // �� Windows ƽ̨ʹ�� "r"
	
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
	_Coordinate result;
	d1.ParseStream(is);
	
	int i = 0;
	//test whether it is writed
	while (!d1["Whether_Write"].GetBool()){
		Sleep(500);		//��ͣ0.5s
		i++;
		if (i % 10 == 0){
			printf("The FromUI.json has not been writed by the UI  ");
			printf("Already pause %ds\n",i/2);
		}
		reopen(2, fp);
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		d1.ParseStream(is);
	}

	//get the _Coordinate
	int x = d1["PositionX"].GetInt();
	int y = d1["PositionY"].GetInt();
	printf("%d%d\n", x, y);
	result.first = x;
	result.second = y;
	fclose(fp);

	//set Whether_Write to false
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "wb"); // �� Windows ƽ̨ʹ�� "r"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	//���߷������ļ���û�б�д���������ܱ���ȡ
	writer.StartObject();
	writer.Key("Whether_Write");
	writer.Bool(false);
	writer.EndObject();
	fclose(fp1);

	return result;
}

void DataTransfer::InitailizeJson(){

	//���ɳ�ʼ����SendToUI.json�ļ������Ұ�Whether_Read����ΪTrue
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // �� Windows ƽ̨ʹ�� "w"
	char writeBuffer[65536];
	FileWriteStream os1(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer1(os1);

	writer1.StartObject();               // Between StartObject()/EndObject(), 

	writer1.Key("Whether_Read");
	writer1.Bool(true);
	writer1.EndObject();
	fclose(fp1);


	//���ɳ�ʼ����FromUI.json�ļ������Ұ�Whether_Read����Ϊfalse
	FILE* fp2 = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "wb"); // �� Windows ƽ̨ʹ�� "w"
	
	FileWriteStream os2(fp2, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer2(os2);

	writer2.StartObject();               // Between StartObject()/EndObject(), 

	writer2.Key("Whether_Write");
	writer2.Bool(false);
	writer2.EndObject();
	fclose(fp2);
}