/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatusNode/TplStatusNode.h>

int main(int argc, char** argv)
{
  TplStatusNode node(argc, argv);
  node.run();
  return 0;
}
