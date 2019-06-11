#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <math.h>
using namespace std;

#define MAX_N 6
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct Grid
{
    int n;
    int i0, j0;
    double hn = 0, gn = 0;
    Grid *parent = NULL;
    int grid[MAX_N][MAX_N];

    Grid(int n) : n(n) {}

    double fn()
    {
        return hn + gn;
    }

    Grid *move(int dir)
    {
        switch (dir)
        {
        case UP:
            if (i0 - 1 < 0)
            {
                return NULL;
            }
            else
            {
                Grid *g1 = new Grid(*this);
                g1->i0 = i0 - 1;
                g1->grid[i0][j0] = grid[i0 - 1][j0];
                g1->grid[i0 - 1][j0] = 0;
                g1->gn = gn + 1;
                g1->parent = this;
                return g1;
            }
            break;
        case DOWN:
            if (i0 + 1 >= n)
            {
                return NULL;
            }
            else
            {
                Grid *g1 = new Grid(*this);
                g1->i0 = i0 + 1;
                g1->grid[i0][j0] = grid[i0 + 1][j0];
                g1->grid[i0 + 1][j0] = 0;
                g1->gn = gn + 1;
                g1->parent = this;
                return g1;
            }
            break;
        case LEFT:
            if (j0 - 1 < 0)
            {
                return NULL;
            }
            else
            {
                Grid *g1 = new Grid(*this);
                g1->j0 = j0 - 1;
                g1->grid[i0][j0] = grid[i0][j0 - 1];
                g1->grid[i0][j0 - 1] = 0;
                g1->gn = gn + 1;
                g1->parent = this;
                return g1;
            }
            break;
        case RIGHT:
            if (j0 + 1 >= n)
            {
                return NULL;
            }
            else
            {
                Grid *g1 = new Grid(*this);
                g1->j0 = j0 + 1;
                g1->grid[i0][j0] = grid[i0][j0 + 1];
                g1->grid[i0][j0 + 1] = 0;
                g1->gn = gn + 1;
                g1->parent = this;
                return g1;
            }
            break;
        default:
            return NULL;
        }
    }

    void locate(int e, int &i, int &j) const
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (grid[i][j] == e)
                {
                    return;
                }
            }
        }
    }

    bool operator==(const Grid &g2)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (g2.grid[i][j] != grid[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }
};

ostream &operator<<(ostream &out, Grid &g)
{
    for (int i = 0; i < g.n; i++)
    {
        for (int j = 0; j < g.n; j++)
        {
            out << g.grid[i][j] << " ";
        }
        out << endl;
    }
    return out;
}

istream &operator>>(istream &in, Grid &g)
{
    for (int i = 0; i < g.n; i++)
    {
        for (int j = 0; j < g.n; j++)
        {
            in >> g.grid[i][j];
            if (g.grid[i][j] == 0)
            {
                g.i0 = i;
                g.j0 = j;
            }
        }
    }
    return in;
}

/**
 * 输出结果
 */
void write_result(Grid &init_grid, Grid *best, ostream &out)
{
    if (best == NULL)
    {
        out << "无解" << endl;
    }
    else
    {
        vector<Grid *> path;
        Grid *g_current = best;
        while (g_current != NULL)
        {
            path.emplace_back(g_current);
            g_current = g_current->parent;
        }
        int steps = path.size() - 1;
        out << "共" << steps << "步" << endl;
        out << "初始状态" << endl;
        out << *(path[steps]);
        for (int i = 1; i < path.size() - 1; i++)
        {
            out << "第" << i << "步" << endl
                << *(path[steps - i]);
        }
        out << "目标状态" << endl
            << *(path[0]) << endl;
    }
}

/**
 * 集合元素查找
 */
Grid *find(set<Grid *> &gs, Grid *gt)
{
    for (auto g : gs)
    {
        if ((*g) == (*gt))
        {
            return g;
        }
    }
    return NULL;
}

/**
 * h(n) = 0
 */
void hn_0(Grid &src, const Grid &dest)
{
    src.hn = 0;
}

/**
 * h(n) = L0范数
 */
void hn_1(Grid &src, const Grid &dest)
{
    int dist = 0;
    for (int i = 0; i < src.n; i++)
    {
        for (int j = 0; j < src.n; j++)
        {
            dist += (src.grid[i][j] != dest.grid[i][j]);
        }
    }
    src.hn = dist;
}

/**
 * h(n) = L1范数
 */
void hn_2(Grid &src, const Grid &dest)
{
    int dist = 0;
    for (int i = 0; i < src.n; i++)
    {
        for (int j = 0; j < src.n; j++)
        {
            if (src.grid[i][j] != dest.grid[i][j] && src.grid[i][j] != 0)
            {
                int i1, j1;
                dest.locate(src.grid[i][j], i1, j1);
                dist += abs(i - i1) + abs(j - j1);
            }
        }
    }
    src.hn = dist;
}

/**
 * h(n) = L2范数
 */
void hn_3(Grid &src, const Grid &dest)
{
    int dist = 0;
    for (int i = 0; i < src.n; i++)
    {
        for (int j = 0; j < src.n; j++)
        {
            if (src.grid[i][j] != dest.grid[i][j] && src.grid[i][j] != 0)
            {
                int i1, j1;
                dest.locate(src.grid[i][j], i1, j1);
                dist += sqrt(pow(i - i1, 2) + pow(j - j1, 2));
            }
        }
    }
    src.hn = dist;
}

/**
 * h(n) = L无穷范数
 */
void hn_4(Grid &src, const Grid &dest)
{
    int dist = 0;
    for (int i = 0; i < src.n; i++)
    {
        for (int j = 0; j < src.n; j++)
        {
            if (src.grid[i][j] != dest.grid[i][j] && src.grid[i][j] != 0)
            {
                int i1, j1;
                dest.locate(src.grid[i][j], i1, j1);
                dist += max(abs(i - i1), abs(j - j1));
            }
        }
    }
    src.hn = dist;
}

/**
 * 查找路径，返回路径尾节点
 */
Grid *find_path(Grid &init_grid, Grid &target_grid, set<Grid *> &open, set<Grid *> &close, void (*hn)(Grid &, Grid &))
{
    // 加入初始节点
    Grid *g_current = new Grid(init_grid);
    Grid *best = NULL;
    g_current->gn = 0;
    (*hn)(*g_current, target_grid);
    open.emplace(g_current);
    // 迭代
    while (open.size() > 0)
    {
        //查找代价最小的状态
        Grid *g_min = NULL;
        for (auto g : open)
        {
            if (g_min == NULL || g_min->fn() > g->fn())
            {
                g_min = g;
            }
        }
        // 把fn最小的元素加入close中
        open.erase(g_min);
        close.emplace(g_min);
        // 检测是否达到目标状态
        g_current = g_min;
        if ((*g_current) == target_grid)
        {
            best = g_current;
            break;
        }

        // 扩展当前节点
        Grid *g_up = g_current->move(UP);
        Grid *g_down = g_current->move(DOWN);
        Grid *g_left = g_current->move(LEFT);
        Grid *g_right = g_current->move(RIGHT);
        auto gs = {g_up, g_down, g_left, g_right};
        for (auto g_new : gs)
        {
            if (g_new == NULL)
            {
                continue;
            }
            // 计算h(n)
            (*hn)(*g_new, target_grid);
            // 查找open和close表
            auto g_close = find(close, g_new);
            auto g_open = find(open, g_new);
            if (g_close == NULL && g_open == NULL)
            {
                open.emplace(g_new);
            }
            else if (g_open != NULL)
            { //在open表中
                if (g_open->fn() > g_new->fn())
                {
                    // 更新节点信息
                    g_open->gn = g_new->gn;
                    g_open->parent = g_new->parent;
                }
                // 回收内存
                delete g_new;
            }
            else
            {
                delete g_new; //已在close表，回收内存
            }
        }
    }

    return best;
}

int main()
{
    ifstream fin("npuzzle_in.txt");
    ofstream fout("npuzzle_out.txt");
    int n;
    fin >> n;
    Grid init_grid(n);
    Grid target_grid(n);
    fin >> init_grid >> target_grid;

    cout << "Init Grid:" << endl;
    cout << init_grid << endl;
    cout << "Target Grid:" << endl;
    cout << target_grid << endl;

    // 遍历所有h(n)
    void *hns[] = {
        (void *)hn_1,
        (void *)hn_2,
        (void *)hn_3,
        (void *)hn_4,
        (void *)hn_0,
    };

    string hn_names[] = {
        "L0   dist",
        "L1   dist",
        "L2   dist",
        "Linf dist",
        "Const   0",
    };

    for (int i = 0; i < sizeof(hns) / sizeof(hns[0]); i++)
    {
        auto hn = (void (*)(Grid &, Grid &))hns[i];
        string name = hn_names[i];
        set<Grid *> open, close;

        cout << name << " searching ..." << endl;
        auto start = chrono::high_resolution_clock::now();

        Grid *best = find_path(init_grid, target_grid, open, close, hn);

        auto end = chrono::high_resolution_clock::now();

        int it = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        cout << "h(n) = " << name << " : Time: " << it / 1000000 << "s " << (it % 1000000) / 1000 << "ms " << it % 1000 << "us, Memory: " << open.size() + close.size() << " (close list: " << close.size() << " open list: " << open.size() << " )\n\n";
        // 输出结果
        // write_result(init_grid, best, cout);
        write_result(init_grid, best, fout);
        // 回收内存
        for (auto g : open)
        {
            delete g;
        }
        for (auto g : close)
        {
            delete g;
        }
    }
    return 0;
}
