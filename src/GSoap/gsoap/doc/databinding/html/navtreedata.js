var NAVTREE =
[
  [ "Data Bindings", "index.html", [
    [ "XML Data Bindings", "index.html", [
      [ "Introduction                                                            ", "index.html#intro", null ],
      [ "Mapping WSDL and XML schemas to C/C++                                   ", "index.html#tocpp", null ],
      [ "Using typemap.dat to customize data bindings                          ", "index.html#typemap", [
        [ "XML namespace bindings                                               ", "index.html#typemap1", null ],
        [ "XSD type bindings                                                    ", "index.html#typemap2", null ],
        [ "Custom serializers for XSD types                                       ", "index.html#custom", [
          [ "xsd:integer", "index.html#custom-1", null ],
          [ "xsd:decimal", "index.html#custom-2", null ],
          [ "xsd:dateTime", "index.html#custom-3", null ],
          [ "xsd:date", "index.html#custom-4", null ],
          [ "xsd:time", "index.html#custom-5", null ],
          [ "xsd:duration", "index.html#custom-6", null ]
        ] ],
        [ "Custom Qt serializers for XSD types                                        ", "index.html#qt", [
          [ "xsd:string", "index.html#qt-1", null ],
          [ "xsd:base64Binary", "index.html#qt-2", null ],
          [ "xsd:hexBinary", "index.html#qt-3", null ],
          [ "xsd:dateTime", "index.html#qt-4", null ],
          [ "xsd:date", "index.html#qt-5", null ],
          [ "xsd:time", "index.html#qt-6", null ]
        ] ],
        [ "Class/struct member additions                                        ", "index.html#typemap3", null ],
        [ "Replacing XSD types by equivalent alternatives                       ", "index.html#typemap4", null ],
        [ "The built-in typemap.dat variables $CONTAINER and $POINTER           ", "index.html#typemap5", null ],
        [ "User-defined content                                                 ", "index.html#typemap6", null ]
      ] ],
      [ "Mapping C/C++ to XML schema                                             ", "index.html#toxsd", [
        [ "Overview of serializable C/C++ types                                   ", "index.html#toxsd1", null ],
        [ "Colon notation versus name prefixing with XML tag name translation     ", "index.html#toxsd2", null ],
        [ "C++ Bool and C alternatives                                            ", "index.html#toxsd3", null ],
        [ "Enumerations and bitmasks                                              ", "index.html#toxsd4", null ],
        [ "Numerical types                                                        ", "index.html#toxsd5", null ],
        [ "String types                                                           ", "index.html#toxsd6", null ],
        [ "Date and time types                                                    ", "index.html#toxsd7", null ],
        [ "Time duration types                                                    ", "index.html#toxsd8", null ],
        [ "Classes and structs                                                    ", "index.html#toxsd9", [
          [ "Serializable versus transient types and data members", "index.html#toxsd9-1", null ],
          [ "Volatile classes and structs", "index.html#toxsd9-2", null ],
          [ "Mutable classes and structs", "index.html#toxsd9-3", null ],
          [ "Default member values in C and C++", "index.html#toxsd9-4", null ],
          [ "Attribute members and backtick XML tags", "index.html#toxsd9-5", null ],
          [ "Qualified and unqualified members", "index.html#toxsd9-6", null ],
          [ "Defining document root elements", "index.html#toxsd9-7", null ],
          [ "(Smart) pointer members and their occurrence constraints", "index.html#toxsd9-8", null ],
          [ "Container members and their occurrence constraints", "index.html#toxsd9-9", null ],
          [ "Tagged union members", "index.html#toxsd9-10", null ],
          [ "Tagged void pointer members", "index.html#toxsd9-11", null ],
          [ "Adding get and set methods", "index.html#toxsd9-12", null ],
          [ "Operations on classes and structs", "index.html#toxsd9-13", null ]
        ] ],
        [ "Special classes and structs                                           ", "index.html#toxsd10", [
          [ "SOAP encoded arrays", "index.html#toxsd10-1", null ],
          [ "XSD hexBinary and base64Binary types", "index.html#toxsd10-2", null ],
          [ "MIME/MTOM attachment binary types", "index.html#toxsd10-3", null ],
          [ "Wrapper class/struct with simpleContent", "index.html#toxsd10-4", null ],
          [ "DOM anyType and anyAttribute", "index.html#toxsd10-5", null ]
        ] ]
      ] ],
      [ "Directives                                                         ", "index.html#directives", [
        [ "Service directives                                               ", "index.html#directives-1", null ],
        [ "Service method directives                                        ", "index.html#directives-2", null ],
        [ "Schema directives                                                ", "index.html#directives-3", null ],
        [ "Schema type directives                                           ", "index.html#directives-4", null ]
      ] ],
      [ "Serialization rules                                                     ", "index.html#rules", [
        [ "SOAP document versus rpc style                                        ", "index.html#doc-rpc", null ],
        [ "SOAP literal versus encoding                                          ", "index.html#lit-enc", null ],
        [ "SOAP 1.1 versus SOAP 1.2                                                 ", "index.html#soap", null ],
        [ "Non-SOAP XML serialization                                           ", "index.html#non-soap", null ]
      ] ],
      [ "Input and output                                                           ", "index.html#io", [
        [ "Reading and writing from/to files and streams                             ", "index.html#io1", null ],
        [ "Reading and writing from/to string buffers                                ", "index.html#io2", null ]
      ] ],
      [ "Memory management                                                      ", "index.html#memory", [
        [ "Memory management in C                                                ", "index.html#memory1", null ],
        [ "Memory management in C++                                              ", "index.html#memory2", null ]
      ] ],
      [ "Context flags to initialize the soap struct                             ", "index.html#flags", null ],
      [ "Context parameter settings                                             ", "index.html#params", null ],
      [ "Error handling and reporting                                           ", "index.html#errors", null ],
      [ "Features and limitations                                             ", "index.html#features", null ],
      [ "Removing SOAP namespaces from XML payloads                              ", "index.html#nsmap", null ],
      [ "Examples                                                             ", "index.html#examples", null ]
    ] ],
    [ "Usage Notes", "page_notes.html", null ],
    [ "XML Data Binding", "page__x_m_l_data_binding.html", [
      [ "Top-level root elements of schema \"urn:address-book-example\"", "page__x_m_l_data_binding.html#a", null ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", null, [
      [ "File List", "files.html", "files" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"address_8cpp.html",
"address_stub_8h.html#a0563f96a962989dec10c451b2dde83bea376d9e4cb40aac7d373c951de94646b6",
"graph_h_8h.html#ad7107b41d774c628aa9536fb8185a8a1"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';