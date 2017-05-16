var express = require('express');
//var app = express();
//var server = require('http').Server(app);
//var io = require('socket.io')(server);
var path = require('path');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
//app.use('/public', express.static(__dirname + '/../UI/public'));
http.listen(85, function() {
    console.log('listening on *:85');
    getBoardFromBackEnd(1);
});
var SOCKET;
//app.listen(85);
app.use(express.static(__dirname + '/public'))
app.get('/', function(req, res) {
    res.sendFile(path.resolve(__dirname + '/public/index.html'));
});
io.on('connection', function(socket) {
    console.log(`connection`);
    socket.emit('connect', 'fuck you');
    socket.on('NextPos', function(data) {
        console.log(data)
        Create_File_Pos(data) //��postioon���뵽��Ӧ���ļ�
        socket.emit('AckPos', 'ACK')
    });
    socket.on('FinishRead', function(data) { //��SendToUI��whether_Read�ĳ�true
        UpdateToUI(data)
        if(data=='user'){
        console.log("Next Round")
        socket.emit('AskAINextRoud','FinishRead')

       }/* if (data == "user") {
            console.log("After finish read, to get the AI board")
            var result = getBoardFromBackEnd(1) //��ȡAI����һ����
            socket.emit('UpdateAIBoard', result)
        }*/

    });
    socket.on('AskUserBoard', function(data) {
        console.log(data + 'ask get board')
        var result = getBoardFromBackEnd(1)
        socket.emit('UpdateUserBoard', result)
    })
    socket.on('AskAIBoard', function(data) {
        console.log(data + 'ask get board')
        var result = getBoardFromBackEnd(1)
        socket.emit('UpdateAIBoard', result)
    })
});

//��ȡAI���͹���������
function getBoardFromBackEnd(Data) {
    var Boardresult;
    while (true) {
        var fs = require('fs');
        var filepath;
        filepath = "./public/data/SendToUI.Json"
            //var file = "d:\\0.json";
        var result = JSON.parse(fs.readFileSync(filepath));
        if (result.Whether_Write) {
            //Boardresult=result.Board
            return result
        }
        sleep(500)
    }
    //�����ݴ���ȥ
    /* if (Data == 1) {
         SOCKET.emit('UpdateAIBoard', result)
     } else {
         SOCKET.emit('UpdateUserBoard', result)
     }*/
}
//����fromui�ļ�����������
function Create_File_Pos(Data) {
    var fs1 = require("fs");
    var data = '{"Whether_Write": true,"PositionX":' + Data[0] + ',"PositionY":' + Data[1] + '}';
    // ����һ������д�������д�뵽�ļ� output.txt ��
    var writerStream = fs1.createWriteStream('./public/data/FromUI.json');
    // ʹ�� utf8 ����д������
    writerStream.write(data, 'UTF8');
    // ����ļ�ĩβ
    writerStream.end();
    // �������¼� --> data, end, and error
    writerStream.on('finish', function() {
        console.log("finsh write FromUI json\n");
    });
    writerStream.on('error', function(err) {
        console.log(err.stack);
    });
}
//UI �Ѿ���ȡ��Board�����ݣ���Ҫ����Ӧ��json�ļ���Whether_Read���True
function UpdateToUI(Data) {
    var fs1 = require("fs");
    var data = '{"Whether_Read": true}'
        // ����һ������д�������д�뵽�ļ� output.txt ��
    var writerStream = fs1.createWriteStream('./public/data/SendToUI.json');
    // ʹ�� utf8 ����д������
    writerStream.write(data, 'UTF8');
    // ����ļ�ĩβ
    writerStream.end();
    // �������¼� --> data, end, and error
    writerStream.on('finish', function() {
        console.log("%s finsh write SendToUI json\n", Data);
    });
    writerStream.on('error', function(err) {
        console.log(err.stack);
    });
    //console.log("Write FromUI json finish\n");
}

//��ʱ����
function sleep(numberMillis) {
    var now = new Date();
    var exitTime = now.getTime() + numberMillis;
    while (true) {
        var j = 0;
        for (var i = 0; i < 100000; i++) {
            j = i + j;
        }
        now = new Date();
        if (now.getTime() > exitTime) break;
    }
    console.log("Pause 100ms\n")
}