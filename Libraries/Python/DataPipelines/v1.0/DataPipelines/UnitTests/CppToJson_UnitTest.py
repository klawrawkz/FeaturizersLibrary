'''Unit test for CppToJson.py
'''
import sys
import json
import unittest
import textwrap

from DataPipelines import CppToJson

class FuncTest(unittest.TestCase):

    def test_add_func(self):
        s = textwrap.dedent('''\
            int add(int a, int b){
                return a+b;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))

        self.assertEqual(func_list[0]['func_name'], 'add')
        self.assertEqual(func_list[0]['raw_return_type'], 'int')
        self.assertEqual(func_list[0]['simple_return_type'], 'int')
        self.assertEqual(func_list[0]['var_names'], ['a', 'b'])
        self.assertEqual(func_list[0]['raw_var_types'], ['int', 'int'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int', 'int'])
        self.assertEqual(func_list[0]['definition_line'], 1)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 4)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_vector_func(self):
        s = textwrap.dedent('''\
            #include <vector>
            std::vector<int> counting(std::vector<float> v, float x){
                std::vector<int>ret;
                for(int i = 0; i < v.size(); i++){
                    if(v[i]==x)
                        ret.push_back(i);
                }
                return ret;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))

        self.assertEqual(func_list[0]['func_name'], 'counting')
        self.assertEqual(func_list[0]['raw_return_type'], 'std::vector<int>')
        self.assertEqual(func_list[0]['simple_return_type'], 'std::vector<int>')
        self.assertEqual(func_list[0]['var_names'], ['v', 'x'])
        self.assertEqual(func_list[0]['raw_var_types'], ['std::vector<float>', 'float'])
        self.assertEqual(func_list[0]['simple_var_types'], ['std::vector<float>', 'float'])
        self.assertEqual(func_list[0]['definition_line'], 2)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 10)
        self.assertEqual(func_list[1]['declaration_line'], None)


    def test_vector_using_std_func(self):
        s = textwrap.dedent('''\
            #include <vector>
            using namespace std;
            vector<int> counting(vector<float> v, float x){
                vector<int>ret;for(int i = 0; i < v.size(); i++){
                    if(v[i]==x)
                        ret.push_back(i);
                }
                return ret;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))

        self.assertEqual(func_list[0]['func_name'], 'counting')
        self.assertEqual(func_list[0]['raw_return_type'], 'vector<int>')
        self.assertEqual(func_list[0]['simple_return_type'], 'vector<int>')
        self.assertEqual(func_list[0]['var_names'], ['v', 'x'])
        self.assertEqual(func_list[0]['raw_var_types'], ['vector<float>', 'float'])
        self.assertEqual(func_list[0]['simple_var_types'], ['vector<float>', 'float'])
        self.assertEqual(func_list[0]['definition_line'], 3)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 10)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_vector_map(self):
        s = textwrap.dedent('''\
            #include <vector>
            #include <map>
            using namespace std;
            vector<vector<int>> counting(vector<map<float,int>> v, float x){
                vector<vector<int>>ret;
                for(int i=0; i<v.size(); i++){
                    if(v[i][x])ret.push_back({1, 2, 3, 4});
                }
                return ret;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))

        self.assertEqual(func_list[0]['func_name'], 'counting')
        self.assertEqual(func_list[0]['raw_return_type'], 'vector<vector<int> >')
        self.assertEqual(func_list[0]['simple_return_type'], 'vector<vector<int> >')
        self.assertEqual(func_list[0]['var_names'], ['v', 'x'])
        self.assertEqual(func_list[0]['raw_var_types'], ['vector<map<float, int> >', 'float'])
        self.assertEqual(func_list[0]['simple_var_types'], ['vector<map<float, int> >', 'float'])
        self.assertEqual(func_list[0]['definition_line'], 4)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 11)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_many_arguments(self):
        s = textwrap.dedent('''\
            int two(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j,
                int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, int u,
                int v, int w, int x, int y, int z){

                return 2;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'two')
        self.assertEqual(func_list[0]['raw_return_type'], 'int')
        self.assertEqual(func_list[0]['simple_return_type'], 'int')
        self.assertEqual(func_list[0]['var_names'], ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'])
        self.assertEqual(func_list[0]['raw_var_types'], ['int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int'])
        self.assertEqual(func_list[0]['definition_line'], 1)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 7)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_many_arguments_ref(self):
        s = textwrap.dedent('''\
            int two(int **a, int **b, int **c, int **d, int **e, int **f, int **g, int **h,
                int **i, int **j, int **k, int **l, int **m, int **n, int **o, int **p, int **q,
                int **r, int **s, int **t, int **u, int **v, int **w, int **x, int **y, int **z){

                return 2;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'two')
        self.assertEqual(func_list[0]['raw_return_type'], 'int')
        self.assertEqual(func_list[0]['simple_return_type'], 'int')
        self.assertEqual(func_list[0]['var_names'], ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'])
        self.assertEqual(func_list[0]['raw_var_types'], ['int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **', 'int **'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int'])
        self.assertEqual(func_list[0]['definition_line'], 1)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 7)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_const_ret(self):
        s = textwrap.dedent('''\
            const float square(float x){
                const float ret = x*x;
                return ret;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'square')
        self.assertEqual(func_list[0]['raw_return_type'], 'const float')
        self.assertEqual(func_list[0]['simple_return_type'], 'float')
        self.assertEqual(func_list[0]['var_names'], ['x'])
        self.assertEqual(func_list[0]['raw_var_types'], ['float'])
        self.assertEqual(func_list[0]['simple_var_types'], ['float'])
        self.assertEqual(func_list[0]['definition_line'], 1)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 5)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_const_arg(self):
        s = textwrap.dedent('''\
            float sum(const float x, const int y){
                return x+y;
            }
            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'sum')
        self.assertEqual(func_list[0]['raw_return_type'], 'float')
        self.assertEqual(func_list[0]['simple_return_type'], 'float')
        self.assertEqual(func_list[0]['var_names'], ['x', 'y'])
        self.assertEqual(func_list[0]['raw_var_types'], ['const float', 'const int'])
        self.assertEqual(func_list[0]['simple_var_types'], ['float', 'int'])
        self.assertEqual(func_list[0]['definition_line'], 1)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 4)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_map_vec_ref(self):
        s = textwrap.dedent( '''\
            #include <vector>
            #include <map>

            using namespace std;

            vector<map<int, float>> * nonsense(vector<int> &v, map<bool, bool>* mp){
                vector<map<int, float>> *ret = new vector<map<int, float>>;
                return ret;
            }

            int main(){
                return 0;
            }
        ''')
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'nonsense')
        self.assertEqual(func_list[0]['raw_return_type'], 'vector<map<int, float> > *')
        self.assertEqual(func_list[0]['simple_return_type'], 'vector<map<int, float> >')
        self.assertEqual(func_list[0]['var_names'], ['v', 'mp'])
        self.assertEqual(func_list[0]['raw_var_types'], ['vector<int> &', 'map<bool, bool> *'])
        self.assertEqual(func_list[0]['simple_var_types'], ['vector<int>', 'map<bool, bool>'])
        self.assertEqual(func_list[0]['definition_line'], 6)
        self.assertEqual(func_list[0]['declaration_line'], None)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 11)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_struct_unsupported(self):
        was_called = False

        s = textwrap.dedent( '''\
            struct x{
                int a, b;
            };

            int go(struct x ya){
                return 2;
            }

            int main(){
                return 0;
            }
        ''')
        # ----------------------------------------------------------------------
        def onUnsupportedFunc(func, filename, line):
            nonlocal was_called
            was_called = True
            self.assertTrue([func, filename, line] in [['go', None, 5], ['main', None, 9]])

        # ----------------------------------------------------------------------

        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, onUnsupportedFunc, lambda type: False))

        self.assertEqual(was_called, True)

        self.assertEqual(func_list, [])


    def test_class_unsupported(self):
        was_called = False

        s = textwrap.dedent('''\
            class Point{
                public:
                    int x, y;
                    Point() {}
                    Point(int a, int b){
                        x=a;
                        y=b;
                    }
            };

            Point operator+ (const Point& a, const Point& b) {
                Point ret;
                ret.x = a.x + b.x;
                ret.y = a.y + b.y;
                return ret;
            }

            int main(){
                return 0;
            }
        ''')
        # ----------------------------------------------------------------------
        def onUnsupportedFunc(func, filename, line):
            nonlocal was_called
            was_called = True
            self.assertTrue([func, filename, line] in [['operator+', None, 11], ['main', None, 18]])

        # ----------------------------------------------------------------------
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, onUnsupportedFunc, lambda type: False))

        self.assertEqual(was_called, True)

        self.assertEqual(func_list, [])

    def test_declaration(self):
        s = textwrap.dedent('''\
            int add(int a, int b);

            int add(int a, int b){
                return a+b;
            }
            int main(){
                return 0;
            }
        ''')


        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'add')
        self.assertEqual(func_list[0]['raw_return_type'], 'int')
        self.assertEqual(func_list[0]['simple_return_type'], 'int')
        self.assertEqual(func_list[0]['var_names'], ['a', 'b'])
        self.assertEqual(func_list[0]['raw_var_types'], ['int', 'int'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int', 'int'])
        self.assertEqual(func_list[0]['definition_line'], 3)
        self.assertEqual(func_list[0]['declaration_line'], 1)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 6)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_namespace_declaration(self):
        s = textwrap.dedent('''\
            namespace DataPipelines {
                namespace Arithmetic {
                    void thisGuy(int *x);
                }
            }

            void DataPipelines::Arithmetic::thisGuy(int *x){
                (*x)++;
            }

            int main(){
                return 0;
            }
        ''')


        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'DataPipelines::Arithmetic::thisGuy')
        self.assertEqual(func_list[0]['raw_return_type'], 'void')
        self.assertEqual(func_list[0]['simple_return_type'], 'void')
        self.assertEqual(func_list[0]['var_names'], ['x'])
        self.assertEqual(func_list[0]['raw_var_types'], ['int *'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int'])
        self.assertEqual(func_list[0]['definition_line'], 7)
        self.assertEqual(func_list[0]['declaration_line'], 3)

        self.assertEqual(func_list[1]['func_name'], 'main')
        self.assertEqual(func_list[1]['raw_return_type'], 'int')
        self.assertEqual(func_list[1]['simple_return_type'], 'int')
        self.assertEqual(func_list[1]['var_names'], [])
        self.assertEqual(func_list[1]['raw_var_types'], [])
        self.assertEqual(func_list[1]['simple_var_types'], [])
        self.assertEqual(func_list[1]['definition_line'], 11)
        self.assertEqual(func_list[1]['declaration_line'], None)

    def test_namespace_func(self):
        s = textwrap.dedent('''\
            #include <stdint.h>

            /* Defined in enclosed namespace */
            namespace DataPipelines {
                namespace Arithmetic {
                    int64_t Add(const int64_t a, const int64_t b) { return a + b; }
                }
            }
        ''')


        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, None, lambda type: True))
        self.assertEqual(func_list[0]['func_name'], 'DataPipelines::Arithmetic::Add')
        self.assertEqual(func_list[0]['raw_return_type'], 'int64_t')
        self.assertEqual(func_list[0]['simple_return_type'], 'int64_t')
        self.assertEqual(func_list[0]['var_names'], ['a', 'b'])
        self.assertEqual(func_list[0]['raw_var_types'], ['const int64_t', 'const int64_t'])
        self.assertEqual(func_list[0]['simple_var_types'], ['int64_t', 'int64_t'])
        self.assertEqual(func_list[0]['definition_line'], 6)
        self.assertEqual(func_list[0]['declaration_line'], None)

    def test_namespace_unsupported(self):
        was_called = False

        s = textwrap.dedent('''\
            #include <stdint.h>

            /* Defined in enclosed namespace */
            namespace DataPipelines {
                namespace Arithmetic {
                    struct MyStruct {
                        int64_t a;
                        int64_t b;
                        MyStruct(int64_t _a = 0, int64_t _b = 0) : a(_a), b(_b) {}
                    };
                    MyStruct Add(const MyStruct& s1, const MyStruct& s2) {
                        return MyStruct(s1.a + s2.a, s1.b + s2.b);
                    }
                }
            }
        ''')


        # ----------------------------------------------------------------------
        def onUnsupportedFunc(func, filename, line):
            nonlocal was_called
            was_called = True
            self.assertEqual(func, "DataPipelines::Arithmetic::Add")
            self.assertEqual(filename, None)
            self.assertEqual(line, 11)

        # ----------------------------------------------------------------------
        func_list = self._GetFuncList(CppToJson.ObtainFunctions(s, onUnsupportedFunc, lambda type: False))

        self.assertEqual(was_called, True)

        self.assertEqual(func_list, [])

    def _GetFuncList(self, results):
        self.assertEqual(len(results), 1)
        self.assertEqual(None, list(results.keys())[0])

        return results[None]


if __name__ == '__main__':
    unittest.main()
