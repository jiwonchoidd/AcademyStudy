#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(vector<string> participant, vector<string> completion) {
    string answer = "";
    for (int j = 0; j < completion.size(); j++)
    {
        for (int i = 0; i < participant.size(); i++)
        {
            if (participant[i] == completion[j])
            {
                participant.push_back(completion[j]);
                participant.erase(participant.begin() + i);
            }
        }
    }
    return participant[0];
}
int main()
{
    vector<string> a = {
        "marina", "josipa", "nikola", "vinko", "filipa"
    };
    vector<string> b = {
        "josipa", "filipa", "marina", "nikola"
    };
    solution(a, b);

    cout << "s";
}
