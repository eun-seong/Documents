function solution(n) {
    var answer = '';

    let r = 0;

    while (n > 0) {
        r = n % 3;

        if (r === 0) r = 4;
        else if (r === 1) r = 1;
        else r = 2;

        answer = String(r).concat(answer);

        n = parseInt((n - 1) / 3);
    }

    return answer;
}