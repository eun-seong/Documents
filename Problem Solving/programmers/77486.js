function solution(enroll, referral, seller, amount) {
    const answer = new Array(enroll.length).fill(0);
    const parent = new Array().fill().map(_ => []);
    const enrollIndex = {};

    enroll.map((e, i) => {
        parent[e] = referral[i];
        enrollIndex[e] = i;
    });


    for (let i = 0; i < seller.length; i++) {
        let cost = amount[i] * 100;
        let cur = seller[i];

        while (true) {
            answer[enrollIndex[cur]] += cost - parseInt(cost * 0.1);
            if (cost < 10) break;

            cur = parent[cur];
            cost = parseInt(cost * 0.1);
        }
    }


    return answer;
}