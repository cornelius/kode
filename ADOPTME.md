The Kode project is up for adoption by a new maintainer. While I still do care
about the project, I don't have the time to do much substantial work on it. It
has not seen a lot of activity recently other than merging the occasional pull
request. A new maintainer is welcome to change that. Taking over maintainership
for only one of the components would also be appreciated and already make a
difference.

Kode consists of three tools:

* kxml_compiler, a code generator for XML parsing and writing
* kxforms, a graphical user interface for editing XML data

They make use of two libraries:

* schema, a library for reading XML Schema
* libkode, a library supporting C++ code generation

kxml_compiler is in use by a number of projects. They rely
on the code generation and schema library. The code generation is fun, the
schema parsing is the harder part. It is an interesting problem to solve,
though.

The compilers work quite reliably, but they are limited in scope, and only
implement a subset of what is possible in XML. Extending this to more use cases
is probably the biggest topic for further development.

kxforms is more of an experiment. It only uses the schema library. The code is a
bit dated, and would need some update to a recent version of Qt. It is a quite
interesting project to play around with.

XML was hot when we started Kode. Today we would likely use JSON for many use
cases instead. Expanding Kode to cover JSON as well would be a quite exciting
next step.

Expanding the test coverage and maybe introducing a CI would add some more peace
of mind and make the job of the maintainer easier. So this might be a step to
take before tackling bigger changes.

But in the end it's up to you, the new maintainer, to decide on what's next and
what suits your needs and the needs of other users best. I'm curious to see what
the future holds for Kode.

Category: Programming

Contact: Cornelius Schumacher <schumacher@kde.org>
