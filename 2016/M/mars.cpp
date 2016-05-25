#include<iostream>
#include<deque>
#include<vector>
#include<tuple>
#include<algorithm>
#include<cstdlib>

using namespace std;

typedef enum {
  COMPUTE = 0,
  LOCK,
  UNLOCK
} i_type;

typedef pair<i_type, int> inst_t;

struct task_t {
  int s;
  int b;
  deque<inst_t> instructions;
  bool complete() { return instructions.size() == 0; }
  bool is_running(int pc) { return !complete() && s <= pc; }
  inst_t next_instruction() { return instructions[0]; }
  inst_t run() {
    inst_t inst = instructions.front();
    instructions.pop_front();
    if (inst.first == COMPUTE && inst.second > 1) {
      inst_t ni = inst;
      ni.second--;
      instructions.push_front(ni);
    }
    return inst;
  }
};

int T, R;
vector<task_t> tasks;

bool lessPriority(int i, int j) { return tasks[i].b < tasks[j].b; }
bool has_task_to_run(vector<task_t> &tasks) {
  for (int i = 0; i < tasks.size(); i++) 
    if (!tasks[i].complete()) return true;
  return false;
}

void simulate()
{
  int pc = 0;
  int resource_ceiling[R];
  bool owned[R];
  int owner[R];
  vector<int> pos;
  int finish_time[T];
  
  for (int i = 0; i < T; i++) pos.push_back(i);
  sort(pos.begin(), pos.end(), lessPriority);
  
  for (int i = 0; i < R; i++) {
    resource_ceiling[i] = 0;
    owned[i] = false;
    owner[i] = -1;
  }

  // calculate the resource_ceiling
  for (int i = 0; i < T; i++) {
    for (int j = 0; j < tasks[i].instructions.size(); j++) {
      inst_t inst = tasks[i].instructions[j];
      if (inst.first == LOCK) {
        resource_ceiling[inst.second] = max(resource_ceiling[inst.second], tasks[i].b);
      }
    }
  }
  
  // event loop
  while (1) {
    if (!has_task_to_run(tasks)) break;
    
    bool blocked[T];
    int priorities[T];

    for (int i = 0; i < T; i++) {
      blocked[i] = false;
      priorities[i] = 0;
    }
    
    // finding the blocked tasks
    for (int i = T - 1; i >= 0; i--) {
      task_t task = tasks[pos[i]];
      priorities[pos[i]] = max(priorities[pos[i]], task.b);
      if (!task.is_running(pc)) continue;
      inst_t ni = task.next_instruction(); 
      if (ni.first == LOCK) {
        if (owned[ni.second]) { // directly blocked by owner[ni]
          blocked[pos[i]] = true;
          priorities[owner[ni.second]] = max(priorities[owner[ni.second]], priorities[pos[i]]);
        }
        for (int j = 0; j < R; j++) { // blocked by owner[j]
          if (owned[j] && owner[j] != pos[i] && resource_ceiling[j] >= priorities[pos[i]]) {
            blocked[pos[i]] = true;
            priorities[owner[j]] = max(priorities[owner[j]], priorities[pos[i]]);
          }
        }
      }
    }
    
    int to_run = -1;
    for (int i = 0; i < T; i++) {
      task_t task = tasks[i];
      if (task.is_running(pc) && !blocked[i]) {
        if (to_run == -1 || priorities[i] > priorities[to_run]) {
          to_run = i;
        }
      }
    }
    
    if (to_run >= 0) {
      // fprintf(stderr, "[%d] RUN %d:", pc, to_run);
      // for (int i = 0; i < tasks[to_run].instructions.size(); i++) {
      //   const char *type = "CLU";
      //   fprintf(stderr, " %c%d", type[tasks[to_run].instructions[i].first], tasks[to_run].instructions[i].second);
      // }
      // fprintf(stderr, "\n");
      // run the command
      inst_t ni = tasks[to_run].run();
      if (ni.first == COMPUTE) {
        pc++;
      } else if (ni.first == LOCK) {
        owned[ni.second] = true;
        owner[ni.second] = to_run;
      } else if (ni.first == UNLOCK) {
        owned[ni.second] = false;
        owner[ni.second] = -1;
      }
      
      if (tasks[to_run].complete()) {
        finish_time[to_run] = pc;
      }
    } else {
      // fprintf(stderr, "[%d] NOP\n", pc);
      pc++;
    }
  }
  
  for (int i = 0; i < T; i++)
    printf("%d\n", finish_time[i]);
}

int main()
{
  cin >> T >> R;
  for (int i = 0; i < T; i++) {
    int s, b, a;
    task_t task;
    cin >> s >> b >> a;
    
    task.s = s;
    task.b = b;
    
    for (int j = 0; j < a; j++) {
      char buf[200];
      cin >> buf;
      inst_t inst;
      if (buf[0] == 'C') inst.first = COMPUTE;
      else if (buf[0] == 'L') inst.first = LOCK;
      else if (buf[0] == 'U') inst.first = UNLOCK;
      else {
        fprintf(stderr, "input %s invalid.\n", buf);
        return -1;
      }
      inst.second = atoi(buf + 1);
      if (inst.first != COMPUTE) inst.second--;
      task.instructions.push_back(inst);
    }
    tasks.push_back(task);
  }
  simulate();
}