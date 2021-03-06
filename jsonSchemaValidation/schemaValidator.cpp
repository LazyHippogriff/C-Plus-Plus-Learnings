/**
 * This program will take the path of a JSON schema and JSON file in the input and check if the JSON file
 * conforms to the schema.
 */

#include "rapidjson/error/en.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <string>

int main(int argc, char * argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s schema.json inputJson.json\n", argv[0]);
    return EXIT_FAILURE;
  }

  rapidjson::Document l_jsonSchemaDoc, l_jsonDoc;

  // Read JSON schema.
  std::ifstream l_jsonSchemaFile(argv[1]);
  std::string l_jsonSchema((std::istreambuf_iterator < char > (l_jsonSchemaFile)), (std::istreambuf_iterator < char > ()));

  l_jsonSchemaDoc.Parse(l_jsonSchema.c_str());
  if (l_jsonSchemaDoc.HasParseError()) {
    fprintf(stderr, "Schema is not a valid JSON\n");
    fprintf(stderr, "Error(offset %u): %s\n",
      static_cast < unsigned > (l_jsonSchemaDoc.GetErrorOffset()),
      GetParseError_En(l_jsonSchemaDoc.GetParseError()));
    return EXIT_FAILURE;
  }

  // Then convert the Document into SchemaDocument
  rapidjson::SchemaDocument sd(l_jsonSchemaDoc);
  rapidjson::SchemaValidator validator(sd);

  //Read JSON which has to be validated
  std::ifstream l_jsonFile(argv[2]);
  std::string l_json((std::istreambuf_iterator < char > (l_jsonFile)), (std::istreambuf_iterator < char > ()));
  l_jsonDoc.Parse(l_json.c_str());
  if (l_jsonDoc.HasParseError()) {
    fprintf(stderr, "Input JSON is not a valid JSON\n");
    fprintf(stderr, "Error(offset %u): %s\n",
      static_cast < unsigned > (l_jsonDoc.GetErrorOffset()),
      GetParseError_En(l_jsonDoc.GetParseError()));
    return EXIT_FAILURE;
  }

  if (!l_jsonDoc.Accept(validator)) {
    printf("\nInput JSON is invalid according to the schema\n");

    // Output diagnostic information
    rapidjson::StringBuffer sb;
    validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
    printf("Invalid schema: %s\n", sb.GetString());
    printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
    sb.Clear();
    validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
    printf("Invalid document: %s\n", sb.GetString());
    return EXIT_FAILURE;
  } else {
    printf("Input JSON is valid according to the schema\n");
    return EXIT_SUCCESS;
  }
}

