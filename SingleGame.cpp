#include "SingleGame.h"

void SingleGame::click(int selectID, int row, int col)
{
    //人机对弈，电脑执黑
    if(!this->RedReady)
     return;

    Board::click(selectID,row,col);//如果人走，调用父类函数

    if(!this->RedReady)
    {
        Step* step=getbestMove();
        MoveStone(step->fromID,step->toRow,step->toCol,step->toID);
    }
}
void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    for(int i=0;i<16;++i)//遍历所有黑棋，0-1为黑棋ID
    {
        if(stone[i].m_dead) continue;//如果棋子死了，跳过
        for(int row=0;row<=9;++row)
        {
            for(int col=0;col<=8;++col)
            {
                int killid=this->getStoneID(row,col);
                if(samecolor(killid,i)) continue;//如果是相同颜色，跳过

                if(canMove(i,row,col,killid))
                {
                    saveStep(i,killid,row,col,steps);//把可能走的步存起来
                }
            }
        }
    }
}
Step* SingleGame::getbestMove()
{
    /*
        1,看看有哪些步骤可以走
        2，试着走一下
        3，评估走的结果
        4，取最好的结果的作为参考
    */
    // 1,看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //2，试着走一下
    int maxScore=-100000;//保存最大的局面分
    Step* ret;
    for(QVector<Step*>::iterator it=steps.begin();it!=steps.end();++it)//使用迭代器遍历所有步数
    {
        Step* step=*it;
        fakeMove(step);//电脑试着走一下，不是真的走
        int score=calcScore();//走一步，计算此时的局面分
        unfakeMove(step);//恢复到原来

        if(score>maxScore)//找出最好的局面分
        {
            maxScore=score;
            ret=step;
        }
    }

    return ret;
    //4，取最好的结果的作为参考

}
void SingleGame::fakeMove(Step *step)
{
    killStone(step->toID);
    MoveStone(step->fromID,step->toRow,step->toCol);//移动棋子
}
void SingleGame::unfakeMove(Step *step)
{
    //复活原来的杀死的棋子
    reliveStone(step->toID);
    MoveStone(step->fromID,step->fromRow,step->fromCol);//移动回原来的位置
}
//评价局面分
int SingleGame::calcScore()
{
    int redTotalScore=0;
    int blackTotalScore=0;
   //为每种棋子设定一个分数，根据计算分值
   // enum TYPE{JIANG,MA,PAO,SHI,RXIANG,BXIANG,BING,ZU,JU,SHUAI};
    static int chessScore[]={1500,50,50,10,10,10,10,10,100,1500};

    //局面分，黑棋分的总数-红棋分的总数
    for(int i=16;i<32;++i)
    {
        if(stone[i].m_dead) continue;
        redTotalScore+=chessScore[stone[i].m_type];//红棋总分
    }
    for(int i=0;i<16;++i)
    {
        if(stone[i].m_dead) continue;
        blackTotalScore+=chessScore[stone[i].m_type];//黑棋总分
    }

    return blackTotalScore-redTotalScore;
}
