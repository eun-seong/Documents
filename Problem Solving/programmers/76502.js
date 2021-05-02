function isCorrect(s) {
    const open = new Array();
    const sz = s.length;

    for (let i = 0; i < sz; i++) {
        if (s[i] === '[' || s[i] === '(' || s[i] === '{') {
            open.unshift(s[i]);
        }
        else {
            if (open.length === 0) return false;
            const c = open[0];
            open.shift();

            if ((c !== '[' && s[i] === ']') ||
                (c !== '(' && s[i] === ')') ||
                (c !== '{' && s[i] === '}')) return false;
        }
    }

    if (open.length !== 0) return false;
    return true;
}

function solution(s) {
    const sz = s.length;
    let answer = 0;

    for (let i = 0; i < sz; i++) {
        if (isCorrect(s)) answer++;
        s = s.concat(s[0]).slice(1);
    }

    return answer;
}