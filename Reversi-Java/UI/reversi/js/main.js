var white_or_black = 2;
var current_board = [[0, 0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 2, 2, 2, 0],
                     [0, 0, 0, 1, 1, 1, 0, 0],
                     [0, 0, 0, 2, 2, 1, 0, 0],
                     [0, 0, 2, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 0]]

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

    // Set Onclick
    $("#main_board .circle").click(function(item) {
        index = parseInt(item.srcElement.id.split('_')[1]);
        i = Math.floor(index / 8);
        j = index % 8;
        console.log(index, i, j)
        current_board[i][j] = white_or_black;
        paintBoard();
    })    
};

function reSetTimer() {
    // Remove alreay paint progressbar
    $("#progress_bar").empty();
    // Begin the gear animation
    $(".gear").css({
        "-webkit-animation-name":"rotate",
        "-webkit-animation-duration":"10s",
        "-webkit-animation-iteration-count":"infinite",
        "-webkit-animation-timing-function":"linear"
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
                setTimeout("alert('You Lost')",300);
                // Stop the gear animation
                $(".gear").css({
                    "-webkit-animation-name":"",
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

function paintBoard() {
    var white_cnt = 0;
    var black_cnt = 0;
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
