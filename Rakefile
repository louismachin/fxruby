require 'rubygems'
require 'hoe'
require 'erb'
require 'rake/extensiontask'
require './lib/fox16/version.rb'

# Some constants we'll need
PKG_VERSION = Fox.fxrubyversion
if RUBY_PLATFORM =~ /mingw/
  FXSCINTILLA_INSTALL_DIR = "c:/src/fxscintilla-1.71/scintilla"
else
  FXSCINTILLA_INSTALL_DIR = "~/src/fxscintilla-1.71/scintilla"
end

hoe = Hoe.spec "fxruby" do
  # ... project specific data ...
  self.blog_categories = %w{FXRuby}
  self.clean_globs = [".config", "ext/fox16/Makefile", "ext/fox16/*_wrap.cxx", "ext/fox16/*_wrap.cpp", "ext/fox16/*.o", "ext/fox16/*.bundle", "ext/fox16/mkmf.log", "ext/fox16/conftest.dSYM", "ext/fox16/include/swigrubyrun.h", "ext/fox16/librb.c"]
  developer("Lyle Johnson", "lyle@lylejohnson.name")
  self.extra_rdoc_files = ["rdoc-sources", File.join("rdoc-sources", "README.rdoc")]
  self.remote_rdoc_dir = "doc/api"
  self.spec_extras = {
    :description => "FXRuby is the Ruby binding to the FOX GUI toolkit.",
    :extensions => ["ext/fox16/extconf.rb"],
    :rdoc_options => ['--main', File.join('rdoc-sources', 'README.rdoc'), '--exclude', 'ext/fox16', '--exclude', %r{aliases|kwargs|missingdep|responder}],
    :require_paths => ['ext/fox16', 'lib'],
    :summary => "FXRuby is the Ruby binding to the FOX GUI toolkit."
  }
  self.test_globs = ["test/**/TC_*.rb"]
  self.version = PKG_VERSION
end

# Make sure extension is built before tests are run
task :test => [:compile]

# The "docs" task created by Hoe assumes that we want to run RDoc
# over everything under the "lib" and "ext" subdirectories.
# We need to go back and tell it to skip the stuff under ext.
# rdoc_target = Rake::Task['docs'].prerequisites.first
# rdoc_files = Rake::Task[rdoc_target].prerequisites
# rdoc_files.reject! {|x| x == "ext/fox16" }

# Make sure that all of the package contents exist before we try to build the package
#Rake::Task['package'].prerequisites.unshift("swig:swig", "fxruby:setversions", "fxruby:generate_kwargs_lib")

# ... project specific tasks ...

Rake::ExtensionTask.new("fox16", hoe.spec) do |ext|
  ext.cross_compile = true
# ext.cross_platform = 'i386-mingw32'
# ext.cross_platform = 'i386-mswin32'
  ext.cross_config_options << "--with-fox-include=/home/lyle/src/mingw/fox-1.6.36/include"
  ext.cross_config_options << "--with-fox-lib=/home/lyle/src/mingw/fox-1.6.36/src/.libs"
  ext.cross_config_options << "--with-fxscintilla-include=/home/lyle/mingw/include/fxscintilla"
  ext.cross_config_options << "--with-fxscintilla-lib=/home/lyle/mingw/lib"
  
  # perform alterations on the gem spec when cross-compiling
  ext.cross_compiling do |gem_spec|
    gem_spec.files.delete "lib/fox16.so"
    gem_spec.files << "lib/1.8/fox16.so"
    gem_spec.files << "lib/1.9/fox16.so"
  end
end

# Make the compile task's list of dependencies begin with the :configure task
Rake::Task['compile'].prerequisites.unshift("fxruby:configure")

#
#  Using the following command-line flags for SWIG:
#
# -nodefaultctor don't generate default constructors for classes that don't declare them
# -nodefaultdtor don't generate default destructors for classes that don't declare them
# -w302 seems to be required to suppress bogus SWIG warnings about redefined identifiers
# -features compactdefaultargs causes SWIG to default to the old code generation method for functions with default arguments
#

namespace :swig do
  SWIG = "swig"
  SWIGFLAGS = "-fcompact -c++ -ruby -nodefaultdtor -nodefaultctor -w302 -features compactdefaultargs -I../fox-includes"
  SWIG_MODULES = {
    "core.i" => "core_wrap.cxx",
    "dcmodule.i" => "dc_wrap.cxx",
    "dialogs.i" => "dialogs_wrap.cxx",
    "framesmodule.i" => "frames_wrap.cxx",
    "iconlistmodule.i" => "iconlist_wrap.cxx",
    "icons.i" => "icons_wrap.cxx",
    "image.i" => "image_wrap.cxx",
    "labelmodule.i" => "label_wrap.cxx",
    "layout.i" => "layout_wrap.cxx",
    "listmodule.i" => "list_wrap.cxx",
    "mdi.i" => "mdi_wrap.cxx",
    "menumodule.i" => "menu_wrap.cxx",
    "fx3d.i" => "fx3d_wrap.cxx",
    "scintilla.i" => "scintilla_wrap.cxx",
    "table-module.i" => "table_wrap.cxx",
    "text-module.i" => "text_wrap.cxx",
    "treelist-module.i" => "treelist_wrap.cxx",
    "ui.i" => "ui_wrap.cxx"
  }

  def wrapper_src_file_path(wrapper_src_file_name)
    File.join("..", "ext", "fox16", wrapper_src_file_name)
  end
  
  def sed(wrapper_src_file_name)
    results = []
    IO.readlines(wrapper_src_file_name).each do |line|
      line.gsub!(/static VALUE mCore;/, "VALUE mCore;")
      line.gsub!(/mCore = rb_define_module\("Core"\)/, "mFox = rb_define_module(\"Fox\")")
      line.gsub!(/mCore/, "mFox")
      next if line =~ /static VALUE m(Dc|Dialogs|Frames|Iconlist|Icons|Image|Label|Layout|List|Mdi|Menu|Fx3d|Scintilla|Table|Text|Treelist|Ui);/
      next if line =~ /m(Dc|Dialogs|Frames|Iconlist|Icons|Image|Label|Layout|List|Mdi|Menu|Fx3d|Scintilla|Table|Text|Treelist|Ui) = rb_define_module/
      next if line =~ /rb_require/
      line.gsub!(/m(Dc|Dialogs|Frames|Iconlist|Icons|Image|Label|Layout|List|Mdi|Menu|Fx3d|Scintilla|Table|Text|Treelist|Ui),/, "mFox,")
      results << line
    end
    File.open(wrapper_src_file_name, "w") do |io|
      io.write(results.join)
    end
  end
  
  def swig(swig_interface_file_name, wrapper_src_file_name)
    system "#{SWIG} #{SWIGFLAGS} -o #{wrapper_src_file_path(wrapper_src_file_name)} #{swig_interface_file_name}"
    sed wrapper_src_file_path(wrapper_src_file_name)
  end

  task :swig_ruby_runtime do
    runtime_filename = wrapper_src_file_path(File.join("include", "swigrubyrun.h"))
    Dir.chdir "swig-interfaces" do
      system "#{SWIG} -ruby -external-runtime #{runtime_filename}"
    end
  end

  desc "Run SWIG to generate the wrapper files."
  task :swig => [:swig_ruby_runtime] do
    Dir.chdir "swig-interfaces" do
      SWIG_MODULES.each do |key, value|
        swig(key, value)
      end
    end
  end
end

namespace :fxruby do

  desc "Update the web site."
  task :website => [:doap] do
    system %{scp -Cq examples/*.rb lyle@rubyforge.org:/var/www/gforge-projects/fxruby/1.6/examples}
    system %{scp -Cq web/index.html lyle@rubyforge.org:/var/www/gforge-projects/fxruby}
    system %{scp -Cq web/community.html lyle@rubyforge.org:/var/www/gforge-projects/fxruby}
    system %{scp -Cq web/documentation.html lyle@rubyforge.org:/var/www/gforge-projects/fxruby}
    system %{scp -Cq web/downloads.html lyle@rubyforge.org:/var/www/gforge-projects/fxruby}
    system %{scp -Cq web/images/* lyle@rubyforge.org:/var/www/gforge-projects/fxruby/images}
    system %{scp -Cq web/css/*.css lyle@rubyforge.org:/var/www/gforge-projects/fxruby/css}
  end

  desc "Upload the DOAP file to the Web site"
  task :doap => [:setversions] do
    system %{scp -Cq doap.rdf lyle@rubyforge.org:/var/www/gforge-projects/fxruby}
  end

  def setversions(filename)
    File.open(filename, "wb") do |out|
      template = ERB.new(File.open(filename + ".erb", "rb").read)
      out.write(template.result)
    end
  end

  desc "Set versions"
  task :setversions do
    setversions("doap.rdf")
  end
  
  def make_impl
    Dir.chdir "ext/fox16" do
      ruby "make_impl.rb"
    end
  end

  task :configure => [:scintilla, :setversions, :generate_kwargs_lib] do
    make_impl
  end

  task :scintilla do
    ruby "scripts/iface.rb -i #{FXSCINTILLA_INSTALL_DIR}/include/Scintilla.iface -o lib/fox16/scintilla.rb"
  end

  task :generate_kwargs_lib do
    ruby 'scripts/generate_kwargs_lib.rb'
  end
end
