# CG


## Todo list

11/17 更新 
1. ~~环境的读取与构建~~  
2. ~~人物模型的读取与构建~~
3. 人物动作的简单实现  ~~11.17更新 人物的旋转还有问题~~ 已解决，发现是glm的translate每次的rotate角度应该被叠加的原因
4. drone的全自动化运动  
5. 碰撞检测的实现  
6. 光照的优化  
7. 材质，颜色的读取 11.8更新：fragshader还是有问题，问题应该出在纹理的读取上，纹理读取不正确导致了绘制的时候颜色不对
8. 视觉效果的改进  ？ 
9. **~~重要，街道的obj无法被绘制！~~** 
10. **~~重要，采用Assimp来对obj进行导入和绘制！~~**  
11. ~~合并之前lab1已经完成的实现与新的lab的实现，~~ ~~将shader与model绑定~~ 更新：shader和model在类中绑定以后发现绘图出现了问题，现在在main中通过对不同的shader进行一样的操作进行绑定，待重构
12. 阴影效果还没有实现，需要对shader进行修改
13. 考虑将改变加速度的函数放在额外的函数里面，还有人物走走停停的频率也放在额外的函数里面，将整个函数在main中判断一下时间间隔，隔一段时间运行一次







10/12 更新  
1. material的读取  
2. ~~drone抖动的问题~~ 采用了改变加速度的方法，暂时不会剧烈抖动了
3. ~~camera的位置不对，现在看不到drone的相对运动~~ 10.9已修复  
4. ~~绘制一个参考平面~~ ，~~添加两个表示起始位置和目标位置的点~~ 10.10已完成，但和模型的位置偏移有点大  
5. ~~代码重构~~ 10.9已修复
6. ~~飞行运动模型有点问题，感觉像是在抖动而不是飞行~~ 10.10已优化飞行模型 
7. ~~drone中多种运动未实现 ，10.10更新：剩余搜索~~ 10.12 已完成
8. 微调参数，使得在飞向一个点的过程中抖动更明显一些
