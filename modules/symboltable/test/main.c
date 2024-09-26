#include "awry/awry.h"
#include "uniform/symboltable/shared.h"

char* symbols[] = {
  "weight", "height", "area", "largestringonright"
};
  
UniformSymbolTable* table;

define_fixture(before, before_all) {
  table = UniformSymbolTableModule.init();
  for (int i = 0; i < 4; i++) {
    UniformSymbolTableModule.insert(table, symbols[i]);
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
      expect(table->root_node->name) to equal("weight")
      expect(table->root_node->left->name) to equal("height")
      expect(table->root_node->left->left->name) to equal("area")
      expect(table->root_node->left->right->name) to equal("largestringonright")
    end
  end

  context(".search")
    when("the node is found")
      it("returns the node")
        UniformSymbolTableNode* n = UniformSymbolTableModule.search(table, "height");
        expect(n->name) to equal("height")
      end
    end

    when("the node is not found")
      it("returns NULL")
        UniformSymbolTableNode* n = UniformSymbolTableModule.search(table, "undefined");
        expect((void*)n) to equal(NULL)
      end
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);

  return 0;
}