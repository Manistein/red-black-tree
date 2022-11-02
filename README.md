# 简介
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;本项目是对红黑树设计和实现的探索。和AVL树相比，红黑树除了会进行旋转，还有复杂的颜色状态改变，因此我们面临的变数会比AVL树多很多，这也是其复杂的原因。但是由于AVL树规定，左右子树的高度差超过1时就要旋转，而红黑树则不然，允许左右子树的高度差相差两倍，因此红黑树比AVL树会有更少的旋转机会，更适合在频繁插入和删除的应用情景。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;网络上已经有很多关于红黑树的优秀文章了，为什么还要写这篇呢？首先，纸上得来终觉浅，只有自己亲自动手实践，才有可能尽最大可能还原事物的真相，这样自己的理解才能更深，本文也是我动手实践之后的梳理和总结。其次，网上众多文章中，确实有相当大一部分缺乏实践的例子，即使有，也很难直观地观察红黑树在插入和删除的变化流程，也不方便发现代码中出现的bug。为此，我专门写了一个图形化的展示工具，用于展示我写的红黑树的效果，如图所示。这个工具姑且就叫red black tree viewer。读者在下载该项目之后，可以在工具中动态添加和删除节点。Red Black Tree Viewer还支持拖动功能，方便在树很深的情况下（会超出窗口）拖动观测。  
![image](https://manistein.github.io/blog/images/data_structure/red-black-tree/red-black-tree-viewer.gif)

# 项目博客
https://manistein.github.io/blog/post/program/c/%E7%BA%A2%E9%BB%91%E6%A0%91%E6%8E%A2%E7%B4%A2/  
http://manistein.club/post/program/c/%E7%BA%A2%E9%BB%91%E6%A0%91%E6%8E%A2%E7%B4%A2/  

# 联系作者
QQ群：185017593
