function checkPos(pos1, pos2) {
    return (pos1[0] == pos2[0]
            && pos1[1] == pos2[1]);
}

function posInArray(pos, array) {
    for (var i = 0; i < array.length; ++i)
        if (checkPos(pos, array[i]))
            return (true);
    return (false);
}

var Direction = {
    UP    : 0,
    DOWN  : 1,
    LEFT  : 2,
    RIGHT : 3
};

var OPPOSITES = new Array;
OPPOSITES[Direction.UP]    = Direction.DOWN;
OPPOSITES[Direction.DOWN]  = Direction.UP;
OPPOSITES[Direction.LEFT]  = Direction.RIGHT;
OPPOSITES[Direction.RIGHT] = Direction.LEFT;

function mod(x, n) {
    return ((x % n) + n) % n;
}

var MOVES = new Array;
MOVES[Direction.UP]    = function(pos, w, h) { pos[1] = mod(pos[1] - 1, h); };
MOVES[Direction.DOWN]  = function(pos, w, h) { pos[1] = mod(pos[1] + 1, h); };
MOVES[Direction.LEFT]  = function(pos, w, h) { pos[0] = mod(pos[0] - 1, w); };
MOVES[Direction.RIGHT] = function(pos, w, h) { pos[0] = mod(pos[0] + 1, w); };

function move(pos, dir, w, h) {
    MOVES[dir](pos, w, h);
}
