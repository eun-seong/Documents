#include <vector>
#include <queue>

using namespace std;

vector<int> solution(int m, int n, vector<vector<int> > picture) {
    int number_of_area = 0;
    int max_size_of_one_area = 0;
    int cur_size = 0;
    int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
    vector<vector<bool> > visited(m, vector<bool>(n, false));
    
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(!visited[i][j] && picture[i][j] != 0){
                number_of_area++;
                int color = picture[i][j];
                cur_size = 1;
                queue<pair<int, int> > q;
                q.push({i, j});
                visited[i][j] = true;
                
                while(!q.empty()){
                    pair<int, int> f = q.front();
                    q.pop();
                    
                    for(int k=0;k<4;k++){
                        int x = f.first+dx[k], y = f.second+dy[k];
                        if(x != -1 && x != m && y != -1 && y != n){
                            if(!visited[x][y] && picture[x][y] == color) {
                                visited[x][y] = true;
                                cur_size++;
                                q.push({x, y});
                            }
                        }
                    }
                }
            }
            if(max_size_of_one_area < cur_size) {
                max_size_of_one_area = cur_size;
            }
        }
    }
    
    vector<int> answer(2);
    answer[0] = number_of_area;
    answer[1] = max_size_of_one_area;
    return answer;
}