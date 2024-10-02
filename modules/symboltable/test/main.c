#include "awry/awry.h"
#include "uniform/symboltable/shared.h"

char* symbols[] = {
  "weight", "height", "area", "largestringonright"
};
  
UniformSymbolTable* table;

define_fixture(before, before_all) {
  table = UniformSymbolTableModule.init();
  for (int i = 0; i < 4; i++) {
    UniformSymbolTableModule.insert_global(table, symbols[i]);
  }
}

define_fixture(after, after_all) {
  UniformSymbolTableModule.clear(table);
}

describe("SymbolTable Test Suite", symboltable_test_suite)
  before(before_all)
  after(after_all)

  context(".insert")
    it("creates the binary tree")
      expect(table->global_node->name) to equal("weight")
      expect(table->global_node->left->name) to equal("height")
      expect(table->global_node->left->left->name) to equal("area")
      expect(table->global_node->left->right->name) to equal("largestringonright")
    end
  end

  context(".search")
    when("the node is found")
      it("returns the node")
        UniformSymbolTableNode* n = UniformSymbolTableModule.search_global(table, "height");
        expect(n->name) to equal("height")
      end
    end

    when("the node is not found")
      it("returns NULL")
        UniformSymbolTableNode* n = UniformSymbolTableModule.search_global(table, "undefined");
        expect((void*)n) to equal(NULL)
      end
    end
  end

  context(".insert_module")
    when("the module has no parent")
      it("gets entered into the root scope")
        char* name = "TestModule";
        UniformSymbolTableNode*  node = malloc(sizeof(UniformSymbolTableNode));
        node->name = malloc(strlen(name) + 1);
        strcpy(node->name, name);

        UniformSymbolTableModule.insert_module(table, node);

        expect(table->global_node->left->left->left->name) to equal(name)
      end
    end

    when("the module has a parent")
      it("gets entered into the module scope")
        char* name = "TestModule";
        char* sub_name = "TestSubModule";

        UniformSymbolTableNode*  node    = malloc(sizeof(UniformSymbolTableNode));
        UniformSymbolTableNode*  subnode = malloc(sizeof(UniformSymbolTableNode));
        subnode->definition.info.module.parent = node;

        node->name    = malloc(strlen(name) + 1);
        subnode->name = malloc(strlen(sub_name) + 1);
        strcpy(node->name, name);
        strcpy(subnode->name, sub_name);

        UniformSymbolTableModule.insert_module(table, node);
        UniformSymbolTableModule.insert_module(table, subnode);

        expect(table->global_node->left->left->left->name) to equal(name)
        expect(node->definition.info.module.symbol_table->global_node->name) to equal(sub_name)
      end
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);

  return 0;
}