Application Design
==========================================

Class Overview
==========================================

.. graphviz::

  digraph {
    rankdir = BT;

    App [ shape="box" ]
    Win32App [ shape="box"]
    ProjectApp [ shape="box"]
    ProjectApp -> Win32App -> App
  }