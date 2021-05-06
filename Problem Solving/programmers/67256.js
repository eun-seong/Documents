function bfs(matrix, from, to) {
    const q = [from];
    const visited = new Array(12).fill(false);
    const dx = [-1, 1, 0, 0], dy = [0, 0, -1, 1];
    let cnt = -1;

    while (q.length != 0) {
        const qSize = q.length;
        cnt++;

        for (let j = 0; j < qSize; j++) {
            const cur = q[0];
            if (cur === to) return cnt;
            q.shift();
            visited[cur - 1] = true;
            const x = parseInt((cur - 1) / 3), y = (cur - 1) % 3;

            for (let i = 0; i < 4; i++) {
                if (x + dx[i] !== -1 && x + dx[i] !== 4 && y + dy[i] !== -1 && y + dy[i] !== 3) {
                    if (!visited[matrix[x + dx[i]][y + dy[i]] - 1]) {
                        q.push(matrix[x + dx[i]][y + dy[i]]);
                    }
                }
            }
        }
    }

    return cnt;
}

function solution(numbers, hand) {
    var answer = '';
    let curLeft = 10, curRight = 12;
    const R = 'R', L = 'L';
    const matrix = new Array(4).fill().map(_ => []);

    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 3; j++) {
            const value = 3 * i + j + 1;
            matrix[i][j] = value;
        }
    }

    numbers.forEach(e => {
        if (e == 0) e = 11;
        if (e === 1 || e === 4 || e === 7) {
            answer = answer.concat(L);
            curLeft = e;
        }
        else if (e === 3 || e === 6 || e === 9) {
            answer = answer.concat(R);
            curRight = e;
        }
        else {
            const rightDist = bfs(matrix, curRight, e);
            const leftDist = bfs(matrix, curLeft, e);
            if (rightDist < leftDist || (rightDist === leftDist && hand === 'right')) {
                curRight = e;
                answer = answer.concat(R);
            }
            else {
                curLeft = e;
                answer = answer.concat(L);
            }
        }
    });

    return answer;
}