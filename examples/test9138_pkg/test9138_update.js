var _xtree = mywindow.findChild("_xtree");
_xtree.addColumn("a", -1, Qt.AlignRight, true, "a");
_xtree.addColumn("b", -1, Qt.AlignRight, true, "b");
_xtree.addColumn("c", -1, Qt.AlignRight, true, "c");
_xtree.addColumn("d", -1, Qt.AlignRight, true, "d");

function query()
{
  try {
    var qry = toolbox.executeDBQuery('test9138_public', 'detail', new Object());
    _xtree.populate(qry);
  }
  catch (e) { print("query exception: " + e); }
}

function print()
{
  toolbox.printReport("test9138_public_report", new Object());
}

mywindow.findChild("_close").clicked.connect(mywindow, "close");
mywindow.findChild("_query").clicked.connect(query);
mywindow.findChild("_print").clicked.connect(print);
