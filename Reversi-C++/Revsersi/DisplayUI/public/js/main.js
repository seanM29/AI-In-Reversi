var white_or_black = 2;
var current_board = [
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 2, 2, 2, 0],
    [0, 0, 0, 1, 1, 1, 0, 0],
    [0, 0, 0, 2, 2, 1, 0, 0],
    [0, 0, 0, 0, 0, 1, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0]
]
var white_cnt = 0;
var black_cnt = 0;
var AI = 2; //black player is 2,
var success = false;
var usersuccess = false;
var SOCKET_URL = 'http://localhost:85/';
var socket = io();

//var socket = io('http://localhost:85/');
//var socket = io(SOCKET_URL);
console.log(socket);
socket.on('connect', function(data1) {
    console.log('connected' + data1);
});
//对方已经收到了落子，确且要求得到新的棋盘
socket.on('AckPos', function(data) {
    socket.emit('AskUserBoard', 'user') //要求得到user落子更新后的棋局
})
socket.on('AskAINextRoud', function(data) {
        socket.emit('AskAIBoard', 'AIPlay')
    })
    //得到数据更新棋盘
socket.on('UpdateUserBoard', function(data) {
        console.log('UpdateUserBoard');
        console.log(data)
        for (var i = 0; i < 64; i++) {
            var row = parseInt(i / 8);
            var col = i % 8;
            current_board[row][col] = data.Board[i];
        }
        //重新绘制棋盘
        paintBoard();
        var audio = document.getElementById("ChessSound")
        audio.play()
        //告诉服务器已经读取
        socket.emit("FinishRead", "user")
        reSetTimer()
        resetTopSignal(2)
    })
    //得到数据更新棋盘
socket.on('UpdateAIBoard', function(data) {
        console.log('UpdateAIBoard');
        console.log(data)
            /* $.each(data.Board, function(i, item) {
                 var row = parseInt(i / 8);
                 var col = i % 8;
                 current_board[row][col] = item;
                 //console.log("row:%d col:%d,数组：%d",row,col,current_board[row][col])
                 console.log(2)
             })*/
        for (var i = 0; i < 64; i++) {
            var row = parseInt(i / 8);
            var col = i % 8;
            current_board[row][col] = data.Board[i];
        }
        paintBoard();
        var audio = document.getElementById("ChessSound")
        audio.play()
        socket.emit("FinishRead", "AI")
        reSetTimer()
        resetTopSignal(1)
    })
    //user play get board data to display
function UserPlay() {
    while (true) {
        console.log(usersuccess)
        console.log("test1\n")
        UsergetData()
        console.log(usersuccess)
        console.log("test2\n")
        if (usersuccess == true) {
            break
        }
        sleep(500)
    }
    usersuccess = false;
    //socket.emit("FinishRead")
    // socket.emit("FinishRead","user")
    reSetTimer()
}
//AI play
function AIPlay() {
    /*  while (true) {
          AIgetData()
          if (success == true) {
              break
          }
          sleep(500)
      }
      success = false;
      //socket.emit("FinishRead","AI")
      reSetTimer()*/
    socket.emit('AskAIBoard', 'AIPlay')
}
//get board data to display
function UsergetData() {
    $.ajaxSettings.async = false;
    $.getJSON("./data/SendToUI.json", function(data) {
        // white_cnt=data.ScoreW
        //black_cnt=data.ScoreB
        console.log(usersuccess)
        console.log("test3\n")
        if (data.Whether_Write) {
            $.each(data.Board, function(i, item) {
                var row = parseInt(i / 8);
                var col = i % 8;
                current_board[row][col] = item;
                //console.log("row:%d col:%d,数组：%d",row,col,current_board[row][col])
                console.log(2)
            })
            paintBoard();
            console.log(usersuccess)
            console.log("test4\n")
            usersuccess = true;
        } else {
            usersuccess = false;
        }
    });
}

function AIgetData() {
    $.ajaxSettings.async = false;
    $.getJSON("./data/SendToUI.json", function(data) {
        // white_cnt=data.ScoreW
        //black_cnt=data.ScoreB
        if (data.Whether_Write) {
            $.each(data.Board, function(i, item) {
                var row = parseInt(i / 8);
                var col = i % 8;
                current_board[row][col] = item;
                //console.log("row:%d col:%d,数组：%d",row,col,current_board[row][col])
                console.log(2)
            })
            console.log("X:%d, Y:%d\n", data.PositionX, data.PositionY)
            paintBoard();
            success = true;
        } else {
            success = false;
        }
    });
}

//初始加载
window.onload = function onLoad() {
    // Show a 'static' progress bar
    var bar = new ProgressBar.Line("#progress_bar", {
        strokeWidth: 1,
        color: '#D7CCC8',
        trailWidth: 0.8,
        trailColor: '#D7CCC8',
        duration: 60000,
        text: {
            value: '60',
            alignToRight: false
        },
    });
    // Set progress bar text's style
    bar.text.style.color = "#8D6E63";
    bar.text.style.top = "15px";
    bar.text.style.left = "240px";
    // getData();
    // Set Onclick
    $("#main_board .circle").click(function(item) {

        //点击函数注册
        index = parseInt(item.srcElement.id.split('_')[1]);
        i = Math.floor(index / 8);
        j = index % 8;
        console.log(index, i, j)
        current_board[i][j] = white_or_black;
        //paintBoard();
        //getData();
        var data = new Array()
        data[0] = i;
        data[1] = j;
        // send postion of human player select to the back end
        console.log("Call NextPos")
        socket.emit('NextPos', data)
            //get the board after human player select
            /* paintBoard();
             console.log(current_board)
             sleep(1000)
             UserPlay();
              console.log(current_board)
             socket.emit("FinishRead","user")
             sleep(1000)
             //get the AI player
             AIPlay()
              console.log(current_board)
              socket.emit("FinishRead","AI")*/
    })
    reSetTimer()
        //AI play first
    if (white_or_black == 2) {
        resetTopSignal(2)
        AIPlay()
        console.log("love you")
    }
};
//根据当前选手选择对应的闪烁图标
function resetTopSignal(data) {
    var white = document.getElementById("white_top_signal")
    var black = document.getElementById("black_top_signal")
        //black.setAttribute("animation-play-state","paused")
    //var classVal_white = document.getElementById("white_top_signal").getAttribute("class");
    //var classVal_black = document.getElementById("black_top_signal").getAttribute("class");
    //now is black player
    if (data == 2) {
        //classVal_white = classVal_white.replace("circle_white_Top", "circle_white");
        white.setAttribute("class", "circle_white");
        //classVal_black = classVal_white.replace("circle_black", "circle_black_Top");
        black.setAttribute("class", "circle_black_Top");
    } else {
       // classVal_black = classVal_black.replace("circle_black_Top", "circle_black");
        black.setAttribute("class", "circle_black");
        // classVal_white = classVal_white.replace("circle_white", "circle_white_Top");
        white.setAttribute("class", "circle_white_Top");
    }
}
//重置计时器
function reSetTimer() {
    // Remove alreay paint progressbar
    $("#progress_bar").empty();
    // Begin the gear animation
    $(".gear").css({
        "-webkit-animation-name": "rotate",
        "-webkit-animation-duration": "10s",
        "-webkit-animation-iteration-count": "infinite",
        "-webkit-animation-timing-function": "linear"
    });
    // Add a progress bar 
    var bar = new ProgressBar.Line("#progress_bar", {
        strokeWidth: 1,
        color: '#8D6E63',
        trailWidth: 0.8,
        trailColor: '#D7CCC8',
        duration: 60000,
        text: {
            value: '',
            alignToRight: false
        },
        step: (state, bar) => {
            var value = 60 - Math.round(bar.value() * 60);
            if (value === 0) {
                bar.stop();
                bar.setText('0');
                setTimeout("alert('You Lost')", 300);
                // Stop the gear animation
                $(".gear").css({
                    "-webkit-animation-name": "",
                });
            } else {
                bar.setText(value);
            }
            bar.text.style.color = state.color;
        }
    });
    // Set progress bar text's style
    bar.text.style.top = "15px";
    bar.text.style.left = "240px";
    bar.animate(1.0);
}

//重新绘制棋盘
function paintBoard() {
    white_cnt = 0;
    black_cnt = 0;
    $("#main_board div").removeClass("inner_circle_white");
    $("#main_board div").removeClass("inner_circle_black");
    for (var i = 0; i < 8; i++) {
        for (var j = 0; j < 8; j++) {
            circle = document.getElementById("circle_" + (i * 8 + j));
            if (current_board[i][j] == 0) {
                circle.setAttribute('class', 'circle');
                continue;
            }
            inner_circle = document.createElement('div');
            if (current_board[i][j] == 1) {
                circle.setAttribute('class', 'circle_white');
                inner_circle.setAttribute('class', 'inner_circle_white');
                circle.appendChild(inner_circle);
                white_cnt++;
            } else {
                circle.setAttribute('class', 'circle_black');
                inner_circle.setAttribute('class', 'inner_circle_black');
                circle.appendChild(inner_circle);
                black_cnt++;
            }
        }
    }
    $("#white_score_digit").text(function() {
        return white_cnt;
    });
    $("#black_score_digit").text(function() {
        return black_cnt;
    });
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


