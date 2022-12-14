# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('entensions'))
sys.path.insert(0, os.path.abspath('entensions'))

primary_domain = 'cpp'

# -- Project information -----------------------------------------------------

project = 'Personal Works'
copyright = '2020, Emmanuel Thivierge'
author = 'Emmanuel Thivierge'

# The full version, including alpha/beta/rc tags
release = '0.0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.

# The graphviz extension use this variable
graphviz_dot = os.path.abspath(".\\..\\..\\..\\Tools\\Graphviz\\dot.exe")
os.environ["GRAPHVIZ_DOT"] = graphviz_dot  # This is necessary for plantuml
plantumlPath = os.path.abspath(
    "./../../../Tools/PlantUML/plantuml.1.2020.18.jar")

#plantuml = 'java -jar %s' % plantumlPath
plantuml = ['java', '-jar', plantumlPath]
plantuml_output_format = 'svg'
extensions = ['sphinx.ext.graphviz', 'sphinxcontrib.plantuml']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'haiku'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
