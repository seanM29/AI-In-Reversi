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


//检查文件是否已经被服务其读取了，防止对还没读取的文件进行写操作
void DataTransfer::CheckRead(int type){
	FILE* fp = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "rb"); // 非 Windows 平台使用 "r"
	
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
	d1.ParseStream(is);
	printf("%d\n", d1["Whether_Read"].GetBool());
	int i = 0;
	
	//若文件没有被读取，则暂停10ms然后重新测试
	while (!d1["Whether_Read"].GetBool()){
		i++;
		if (i % 50 == 0){
			printf("Type:%d The SendToUI.json has not been read by the UI# ",type);
			printf("Already pause %ds\n",i/10);
		}
		Sleep(100);		//暂停10ms
		
		reopen(1, fp);
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		d1.ParseStream(is);
	}
	fclose(fp);
}


//拷贝Board用来传输
void DataTransfer::UpdateBoard(int b[8][8]){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			transfer_board[i][j] = b[i][j];
		}
	}
}
//人类选手下棋，后台更新好后返回给UI显示
bool DataTransfer::SendToUI(int b[8][8]){
	
	CheckRead(2);

	UpdateBoard(b);

	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // 非 Windows 平台使用 "w"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	
	//将数据写入json文件中国
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
	
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // 非 Windows 平台使用 "w"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	//将数据写入json文件中
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
	//重新大开sendUI文件
	if (type == 1){
		fclose(fp);
		fp = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "rb");
	}
	else{ //打开fromUI文件
		fclose(fp);
		fp = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "rb"); // 非 Windows 平台使用 "r"
	}

}

_Coordinate DataTransfer::GetFromUI(){
	FILE* fp = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "rb"); // 非 Windows 平台使用 "r"
	
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
	_Coordinate result;
	d1.ParseStream(is);
	
	int i = 0;
	//test whether it is writed
	while (!d1["Whether_Write"].GetBool()){
		Sleep(500);		//暂停0.5s
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
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "wb"); // 非 Windows 平台使用 "r"
	char writeBuffer[65536];
	FileWriteStream os(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	//告诉服务器文件还没有被写过，还不能被读取
	writer.StartObject();
	writer.Key("Whether_Write");
	writer.Bool(false);
	writer.EndObject();
	fclose(fp1);

	return result;
}

void DataTransfer::InitailizeJson(){

	//生成初始化的SendToUI.json文件，并且把Whether_Read设置为True
	FILE* fp1 = fopen(".\\DisplayUI\\public\\data\\SendToUI.json", "wb"); // 非 Windows 平台使用 "w"
	char writeBuffer[65536];
	FileWriteStream os1(fp1, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer1(os1);

	writer1.StartObject();               // Between StartObject()/EndObject(), 

	writer1.Key("Whether_Read");
	writer1.Bool(true);
	writer1.EndObject();
	fclose(fp1);


	//生成初始化的FromUI.json文件，并且把Whether_Read设置为false
	FILE* fp2 = fopen(".\\DisplayUI\\public\\data\\FromUI.json", "wb"); // 非 Windows 平台使用 "w"
	
	FileWriteStream os2(fp2, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer2(os2);

	writer2.StartObject();               // Between StartObject()/EndObject(), 

	writer2.Key("Whether_Write");
	writer2.Bool(false);
	writer2.EndObject();
	fclose(fp2);
}