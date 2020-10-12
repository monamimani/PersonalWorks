Architecture and Design Overview
==========================================

.. graphviz::

   strict digraph {
     rankdir = BT;
     
     Project  [ shape="box" ]
     App [ shape="box" ]
     Core [ shape="box" ]
     InputSystem [ shape="box" ]
     Gfx  [ shape="box" ]
     GfxHAL [ shape="box" ]
     VKRHI [ shape="box" ]
     
     {App InputSystem Gfx GfxHAL VKRHI} -> Core
     App -> InputSystem
     Project -> App
     Project -> {Gfx GfxHAL}
   }

.. uml:: ArchitectureLayerOverview.uml

.. uml:: ArchitectureOverview.uml