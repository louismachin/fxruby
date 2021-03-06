require 'test/unit'
require 'fox16'
require 'testcase'

include Fox

class TC_FXId < TestCase
  def setup
    super(self.class.name)
  end

  def test_created?
    assert !mainWindow.created?
    app.create
    assert mainWindow.created?, "main window should be created after call to FXApp#create"
    mainWindow.destroy
    assert !mainWindow.created?
  end
end
