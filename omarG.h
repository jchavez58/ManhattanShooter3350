/*
class box {
    private:
	int top;
	int bottom;
	int left;
	int right;


}; */
extern void Drawbullets(Bullet*, Global&);
extern void ShootBullets(Global&, Bullet*, Timers&);
extern void movecharUp(Global&);
extern void movecharDown(Global&);
extern void moveForward(Global&);
extern void moveBack(Global&);
extern void detectCharWallColission(int,int,Global&);
extern void EnemyLoop(Global&);
extern void spawnEnemy(Global&, const float, const float);
extern float RandomizeEnemyPosx();
extern float moveEnemy(Global&);
extern float RandomizeEnemyPosx();

