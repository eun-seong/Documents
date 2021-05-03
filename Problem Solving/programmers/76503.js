function solution(a, edges) {
    let answer = BigInt(0);
    const nodeNum = a.length;
    const nodeList = new Array(nodeNum).fill().map(_ => []);
    const visited = new Array(nodeNum).fill(false);
    const nodes = new Array(nodeNum);
    let stack = [[0, -1]];

    if (a.reduce((c, e) => c + e) != 0) return -1;

    for (let [a, b] of edges) {
        nodeList[a].push(b);
        nodeList[b].push(a);
    }

    while (stack.length != 0) {
        let [current, parent] = stack.pop();

        if (visited[current]) {
            answer += BigInt(Math.abs(a[current]));
            a[parent] += a[current];
            a[current] = 0;
            continue;
        }

        stack.push([current, parent]);
        visited[current] = true;

        for (let child of nodeList[current]) {
            if (!visited[child]) stack.push([child, current]);
        }
    }


    return answer;
}