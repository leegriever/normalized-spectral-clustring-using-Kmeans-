from setuptools import setup, find_packages, Extension

module = Extension("spkmeansmodule",
                   sources=["spkmeansmodule.c", "kmeans.c",
                            "spkmeans.c"])
setup(
    name = 'spkmeansmodule',
    version = '0.1.0',
    author = "Lee and Shira",
    author_email = "leegriever@mail.tau.ac.il",
    description = "SPKMeans",
    install_requires = ['invoke'],
    packages = find_packages(),

    license = 'GPL_2',
    classifiers = [
        'Development Status :: 3 - Alpha',
        'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
        'Natural Language :: Python :: 3 :: Only',
        'Programming Language :: Python :: Implementation :: CPython',
    ],
    ext_modules = [module]
)