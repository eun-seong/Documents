function solution(record) {
    var answer = [];
    const users = new Map();
    const stmtList = [];

    record.forEach(e => {
        const stmt = e.split(' ');
        if (stmt[0] === 'Enter') {
            users.set(stmt[1], stmt[2]);
            stmtList.push([stmt[0], stmt[1]]);
        }
        else if (stmt[0] === 'Change') {
            users.set(stmt[1], stmt[2]);
        }
        else if (stmt[0] === 'Leave') {
            stmtList.push(stmt);
        }
    });

    stmtList.forEach(e => {
        const userName = users.get(e[1]);
        if (e[0] === 'Enter') {
            answer.push(userName.concat("님이 들어왔습니다."));
        }
        else {
            answer.push(userName.concat("님이 나갔습니다."));
        }
    });
    return answer;
}