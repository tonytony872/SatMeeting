#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include "sat.h"

using namespace std;

class Gate
{
public:
   Gate(unsigned i = 0): _gid(i) {}
   ~Gate() {}

   Var getVar() const { return _var; }
   void setVar(const Var& v) { _var = v; }

private:
   unsigned   _gid;  // for debugging purpose...
   Var        _var;
};


vector<Gate *> gates;
vector<Var*> wire2gates;
void
initCircuit(string file, SatSolver& s)
{
   // Init gates
   ifstream infile(file);
   string line;
   stringstream ss;
   int numvar=1;
   gates.push_back(new Gate(0));// useless gate
   while(getline(infile, line)){
      ss.str("");
      ss.clear();
      ss<<line;
      string gate;
      ss>>gate;
      // cerr<<gate<<endl;
      if (gate=="total_route"){
         int num_wire;
         ss>>num_wire;
         wire2gates.resize(num_wire);
      }
      else if(gate=="OW"){/*TWO INPUTS*/}
      else if(gate=="F"){
         /*3 INPUTS*/
         int a=0, b=0, c=0;
         ss>>a; ss>>b; ss>>c;
         gates.push_back(new Gate(numvar));
         Var v=s.newVar();
         gates.back()->setVar(v);
         wire2gates[a]=new Var(numvar);
         wire2gates[b]=new Var(numvar);
         wire2gates[c]=new Var(numvar);
         numvar++;//1,4,7....
      }
      else if(gate=="ON"){
         int a=0, b=0;
         ss>>a;
         Gate* g=gates[*wire2gates[a]];
         bool flag= (a%3==0)? true : false ;//negation is times of 3
         while(ss>>b){
            gates.push_back(new Gate(numvar));
            wire2gates[b]=new Var(numvar);
            Var v=s.newVar();
            gates.back()->setVar(v);
            Var new_v=s.newVar();
            // cout<<b<<","<<*wire2gates[a]<<","<<flag<<endl;
            s.addOrCNF(new_v, gates[*wire2gates[b]]->getVar(), 1, g->getVar(), flag);
            s.assumeProperty(new_v, true);
            numvar++;
         }
      }
      else if(gate=="XOR"){
         /*2*/
         int x=0, y=0;
         ss>>x; ss>>y;
         Var v=s.newVar();
         s.addOrCNF(v, gates[*wire2gates[x]]->getVar(), 1, gates[*wire2gates[y]]->getVar(), 1);
         s.assumeProperty(v, true);
      }
      else if(gate=="H"){
         /*4 INPUTS, but 3 now*/
         int a=0, b=0, c=0;
         ss>>a; ss>>b; ss>>c;
         Var v=s.newVar();
         // cout<<gates[*wire2gates[a]]->getVar()<<","<<gates[*wire2gates[b]]->getVar()<<endl;
         s.addOrCNF(v, gates[*wire2gates[a]]->getVar(), 0, gates[*wire2gates[b]]->getVar(), 0);
         gates.push_back(new Gate(numvar));
         gates.back()->setVar(v);
         wire2gates[c]=new Var(numvar);
         numvar++;
      }
      else if(gate=="L"){
         /*4*/
         int a=0, b=0, c=0;
         ss>>a; ss>>b; ss>>c;
         // if(b==0){
         //    s.assumeRelease();
         s.assumeProperty(gates[*wire2gates[c]]->getVar(), true); // true: 結果為1叫做SAT
         // }
      }
      else{/*what case??*/}
   }
}

void
genProofModel(SatSolver& s)
{
   // Allocate and record variables; No Var ID for POs
   for (size_t i = 0, n = gates.size(); i < n; ++i) {
      Var v = s.newVar();
      gates[i]->setVar(v);
   }
}

void reportResult(const SatSolver& solver, bool result)
{
   solver.printStats();
   cout << (result? "SAT" : "UNSAT") << endl;
   if (result) {
      for (size_t i = 1, n = gates.size(); i < n; ++i)
         cout << solver.getValue(gates[i]->getVar()) << " ";
   }
   cout<<endl;
}

int main(int argc, char** argv)
{
   if(argc!=2){cout<<"no input file detected!\n";return 1;}

   SatSolver solver;
   solver.initialize();

   initCircuit(argv[1], solver);

   //
   // genProofModel(solver);

   bool result;
   // reportResult(solver, result);
   // k = Solve(Gate(5) ^ !Gate(8))
   // Var newV = solver.newVar();
   // solver.addXorCNF(newV, gates[5]->getVar(), false, gates[8]->getVar(), true);
   // solver.assumeRelease();  // Clear assumptions
   // solver.assumeProperty(newV, true);  // k = 1
   result = solver.assumpSolve();
   reportResult(solver, result);

   cout << endl << endl << "======================" << endl;

   // k = Solve(Gate(3) & !Gate(7))
   // newV = solver.newVar();
   // solver.addAigCNF(newV, gates[3]->getVar(), false, gates[7]->getVar(), true);
   // solver.assumeRelease();  // Clear assumptions
   // solver.assumeProperty(newV, true);  // k = 1
   // result = solver.assumpSolve();
   // reportResult(solver, result);
}