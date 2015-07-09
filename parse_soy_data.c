#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

/** Given an xmlDocPtr "doc" and an xmlNotePtr "cur" that points to an
 * "observation" element, print out the information contained in that
 * observation element. */
void
display_observation(xmlDocPtr doc, xmlNodePtr cur) {

    xmlChar *year, *day, *hour, *solar, *temp, *units, *rh, *wind, *precipitation;

    year = xmlGetProp(cur, "year");
    day = xmlGetProp(cur, "day-of-year");
    hour = xmlGetProp(cur, "hour");

    printf("Observation at hour %s of day %s of %s:\n", hour, day, year);

    /* We're done with these xmlChar objects. */
    xmlFree(year);
    xmlFree(day);
    xmlFree(hour);


    cur = cur->xmlChildrenNode;
    while (cur != NULL) {

        /* Because we use pretty-formatted out XML documented with nice
         * indentation, we have to use this test to ignore all the child nodes of
         * <observation> that AREN'T elements--namely, all of the text nodes
         * corresponding to the whitespace we introduced by pretty-formatting.  If
         * we'd produced XML that looks like this:
         *
         * <?xml version="1.0" encoding="UTF-8"?><data-set><observation year="2002" day-of-year="1" hour="0"><solar>0.000000</solar><temp units="°C">-13.050000</temp><rh>0.802061</rh><wind>3.118104</wind><precipitation>0.000000</precipitation></observation></data-set>,
         *
         * we wouldn't need this test. */
        if (cur->type == XML_ELEMENT_NODE) {
            
            /* Note that it doesn't matter what order the parameter variables
             * are in.  On the other hand, there is no check in the code for
             * duplicate or missing variables.  We do check for unexpected
             * elements, however. */
            if (xmlStrEqual(cur->name, "solar")) {
                solar = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                printf("\tsolar: %s\n", solar);
                xmlFree(solar);
            }
            else if (xmlStrEqual(cur->name, "temp")) {
                temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                units = xmlGetProp(cur, "units");
                printf("\ttemp: %s%s\n", temp, units);
                xmlFree(temp);
            }
            else if (xmlStrEqual(cur->name, "rh")) {
                rh = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                printf("\trh: %s\n", rh);
                xmlFree(rh);
            }
            else if (xmlStrEqual(cur->name, "wind")) {
                wind = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                printf("\twind: %s\n", wind);
                xmlFree(wind);
            }
            else if (xmlStrEqual(cur->name, "precipitation")) {
                precipitation = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                printf("\tprecipitation: %s\n", precipitation);
                xmlFree(precipitation);
            }
            else {
                printf("UNEXPECTED ELEMENT!");
                exit(1);
            }
      
        }
        cur = cur->next;
    }

    printf("\n");
    return;
}

static void
parseDoc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "data-set")) {
		fprintf(stderr,"document of the wrong type, root node != data-set");
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "observation"))){
			display_observation(doc, cur);
		}

        cur = cur->next;
	}

	xmlFreeDoc(doc);
	return;
}

int
main(int argc, char **argv) {

	char *docname;

	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(1);
	}

	docname = argv[1];
	parseDoc (docname);

	return (0);
}
