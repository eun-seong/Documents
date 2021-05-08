function solution(new_id) {
    new_id = new_id.toLowerCase().replace(/[\~\!\@\#\$\%\^\&\*\(\)\=\+\[\{\]\}\:\?\,\<\>\/]/gi, '')
        .replace(/\.\.+/gi, '.')
        .replace(/^\./gi, '').replace(/\.$/gi, '');  // => .replace(/^\.|\.$/gi, '');

    if (new_id === '') new_id = 'a'; // => .replace(/^$/, '');
    if (new_id.length > 15) new_id = new_id.substr(0, 15).replace(/\.$/gi, '');  // => .slice(0, 15).replace(/\.$/gi, '');
    while (new_id.length <= 2) {
        new_id = new_id.concat(new_id[new_id.length - 1]);
    }   // => new_id = new_id.padEnd(3, new_id[new_id.length-1]);

    return new_id;
}