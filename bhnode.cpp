#include "common.h"
#include "bhnode.hpp"

/*
  cposから見たposのインデックスを返す関数
  0bxyz でインデックスが定まる
*/
int CalcChildIndex(Vector3 pos, Vector3 cpos)
{
  int subindex = 0;
  for (int k = 0; k < 3; k++)
  {
    subindex <<= 1;
    if (pos[k] > cpos[k])
      subindex += 1;
  }
  return subindex;
}


/*
  pfirst -> p[0] -> p[1] ...
  のようなリストを作成し、メンバ変数pfirstに格納する関数
  全てのパーティクルを持つ根ノードを作成している
*/
void BHNode::AssignRoot(Vector3 root_pos, double length, Particle *p, int np)
{
  pos = root_pos;
  size = length;
  pfirst = p;
  nparticle = np;
  for (int i = 0; i < np - 1; i++)
  {
    p->next = p + 1;
    p++;
  }
  p->next = NULL;
}

/*
  親ノードから再帰的に粒子を子ノードに割り当てていく関数
*/
void BHNode::CreateTreeRecursive(BHNode* &heap_top, int &heap_remainder)
{
  for (int i = 0; i < 8; i++)
    child[i] = NULL;
  Particle *p = pfirst;
  // このノードが持つ全てのパーティクルについて操作
  for (int i = 0; i < nparticle; i++)
  {
    Particle *pnext = p->next;
    int sub_index = CalcChildIndex(p->pos, centerPos);
    AssignChild(sub_index, heap_top, heap_remainder);
    child[sub_index]->nparticle++;
    p->next = child[sub_index]->pfirst;
    child[sub_index]->pfirst = p;
    p = pnext;
  }
  for (int i = 0; i < 8; i++)
    if (child[i] != NULL)
    {
      if (child[i]->nparticle > 1)
        child[i]->CreateTreeRecursive(heap_top, heap_remainder);
      else
      {
        child[i]->pos = child[i]->pfirst->pos;
        child[i]->mass = child[i]->pfirst->mass;
      }
    }
}

/*
  まだその子ノードに粒子が登録されていない場合、
  新しいBHNodeを割り当て、そこにその粒子を登録する関数
  割り当てられるBHNodeがもうない場合はエラーを返す
*/
void BHNode::AssignChild(int subindex, BHNode* &heap_top, int &heap_remainder)
{
  if (heap_remainder <= 0)
  {
    cerr << "create_tree: no more free node... exit\n";
    exit(1);
  }
  if (child[subindex] == NULL)
  {
    child[subindex] = heap_top;
    heap_top++;
    heap_remainder--;
    child[subindex]->centerPos = centerPos +
                            Vector3(
                                ((subindex & 4) * 0.5 - 1) * size / 4, // !!(subindex & 4)でよさそう
                                ((subindex & 2) - 1) * size / 4,
                                ((subindex & 1) * 2 - 1) * size / 4);
    child[subindex]->size = size * 0.5;
    child[subindex]->nparticle = 0;
  }
}

void BHNode::DumpTree(int indent)
{
  int i;
  for (int i = 0; i < indent; i++) cerr << " ";

  cerr << "Node center pos(" << centerPos << ") ";
  if (nparticle == 1)
  {
    cerr << "(LEAF) ";
    PRL(nparticle);
    Particle *p = pfirst;
    for (i = 0; i < nparticle; i++)
    {
      for (int j = 0; j < indent + 2; j++) cerr << " ";
      PRL(p->pos);
      p = p->next;
    }
  }
  else
  {
    PRL(nparticle);
    for (i = 0; i < 8; i++)
    {
      if (child[i] != NULL)
      {
        child[i]->DumpTree(indent + 2);
      }
    }
  }
}

/*
  再帰的にノードの重心・質量を計算し、設定する関数
*/
void BHNode::CalcPhysicalQuantity()
{
  if (nparticle > 1)
  {
    int i;
    pos = Vector3();
    mass = 0.0;
    for (i = 0; i < 8; i++)
    {
      if (child[i] != NULL)
      {
        child[i]->CalcPhysicalQuantity();
        double mchild = child[i]->mass;
        pos += mchild * child[i]->pos;
        mass += mchild;
      }
    }
    pos /= mass;
  }
}
