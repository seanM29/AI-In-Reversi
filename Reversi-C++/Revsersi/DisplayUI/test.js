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
        Create_File_Pos(data) //将postioon存入到对应的文件
        socket.emit('AckPos', 'ACK')
    });
    socket.on('FinishRead', function(data) { //将SendToUI的whether_Read改成true
        UpdateToUI(data)
        if(data=='user'){
        console.log("Next Round")
        socket.emit('AskAINextRoud','FinishRead')

       }/* if (data == "user") {
            console.log("After finish read, to get the AI board")
            var result = getBoardFromBackEnd(1) //读取AI的下一步棋
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

//读取AI发送过来的数据
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
    //将数据传回去
    /* if (Data == 1) {
         SOCKET.emit('UpdateAIBoard', result)
     } else {
         SOCKET.emit('UpdateUserBoard', result)
     }*/
}
//创建fromui文件，放入坐标
function Create_File_Pos(Data) {
    var fs1 = require("fs");
    var data = '{"Whether_Write": true,"PositionX":' + Data[0] + ',"PositionY":' + Data[1] + '}';
    // 创建一个可以写入的流，写入到文件 output.txt 中
    var writerStream = fs1.createWriteStream('./public/data/FromUI.json');
    // 使用 utf8 编码写入数据
    writerStream.write(data, 'UTF8');
    // 标记文件末尾
    writerStream.end();
    // 处理流事件 --> data, end, and error
    writerStream.on('finish', function() {
        console.log("finsh write FromUI json\n");
    });
    writerStream.on('error', function(err) {
        console.log(err.stack);
    });
}
//UI 已经读取了Board的数据，需要将对应的json文件的Whether_Read变成True
function UpdateToUI(Data) {
    var fs1 = require("fs");
    var data = '{"Whether_Read": true}'
        // 创建一个可以写入的流，写入到文件 output.txt 中
    var writerStream = fs1.createWriteStream('./public/data/SendToUI.json');
    // 使用 utf8 编码写入数据
    writerStream.write(data, 'UTF8');
    // 标记文件末尾
    writerStream.end();
    // 处理流事件 --> data, end, and error
    writerStream.on('finish', function() {
        console.log("%s finsh write SendToUI json\n", Data);
    });
    writerStream.on('error', function(err) {
        console.log(err.stack);
    });
    //console.log("Write FromUI json finish\n");
}

//延时函数
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