# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
### Added
- Added support for decoding HTML entities (decode_html_entities()) (2018-09-28).
- Added serialisation through the [cereal](http://uscilab.github.io/cereal/index.html) C++ library. Library headers have been copied to src/CDM/cereal (2017-02-26).
- Added UNICODE normalisation (NFC) through boost::locale.
- Changed unsigned long to uint32\_t.
- Allowed Tcl functions \(tip\_\*\) to accept either CDM objects wrapped as Tcl value objects, or as SWIG objects. For example, all "tip\_GetType [CDM::Annotation -args token]", "tip\_GetType [CDM::Annotation ann token]" and "tip\_GetType {{} token {} {}}" are valid.

### Changed
### Deprecated
### Removed
### Fixed
### Security
