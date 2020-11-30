import os
import sys
sys.path.insert(0, os.path.abspath('../'))


# -- Project information -----------------------------------------------------
project = 'Lovejoy Compiler'
copyright = '2020, Sammy'
author = 'Sammy'

doxy_dir = './xml'
breathe_projects = { 'ljc': doxy_dir }
breathe_default_project = 'ljc'
breathe_implementation_filename_extensions = []

release = '0.1.0'

primary_domain = 'c'
highlight_language = 'c'

extensions = [
    'breathe',
    'exhale'
]

exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "root.rst",
    "rootFileTitle":         "Lovejoy Compiler API",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    "exhaleUseDoxyfile": True,
    #"exhaleDoxygenStdin": "INPUT = ../src"
}

templates_path = ['_templates']

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'alabaster'
#html_theme = 'bizstyle'
#html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_css_files = [
    'styles.css',
    'fonts.css'
]
html_theme_options = {
    'github_user': 'Demonstrandum',
    'github_repo': 'ljc',
    'github_type': 'star',
    'github_count': 'true'
}
