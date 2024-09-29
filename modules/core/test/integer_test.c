#include "awry/awry.h"
#include "uniform/core/shared.h"

UniformSymbolTable* table;

define_fixture(before, before_all) {
  table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(table);
}

define_fixture(after, after_all) {
  UniformSymbolTableModule.clear(table);
}

describe("Uniform Core Integer Test Suite", coreint_test_suite)
  before(before_all)
  after(after_all)

  context(".init")
    it("adds the Integer type")
      UniformSymbolTableNode* n = UniformSymbolTableModule.search_global(table, UNIFORM_INTEGER_TYPE);
      expect((void*)n) to not be_null
      expect(n->name) to equal(UNIFORM_INTEGER_TYPE)
      expect(n->type->size) to equal(sizeof(int))
      expect(n->type->form) to equal(UNIFORM_SCALAR_FORM)
      expect((void*)(n->type->type_idp)) to equal((void*)n)
    end
  end
end