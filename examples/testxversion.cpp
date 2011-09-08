#include <stdlib.h>
#include <QString>

#include <stdio.h>

#include "xversion.h"

char *formattests[] = {
  "",
  "1",
  "1.",
  "1.0",
  "1.2",
  "1.2wip",
  "1.2alpha",
  "1.2beta",
  "1.2gamma",
  "1.2rc",
  "1.23",
  "1.23.0",
  "0.0.0rc2",
  "1.2.3",
  "1.2.3wip",
  "1.2.3wip1",
  "1.2.3alpha",
  "1.2.3alpha1",
  "1.2.3alpha2",
  "1.2.3beta",
  "1.2.3beta0",
  "1.2.3beta1",
  "1.2.3beta2",
  "1.2.3rc-1",
  "1.2.3rc",
  "1.2.3rc0",
  "1.2.3rc4",
  "1.2.3rc4invalid",
  "1.2.3rcinvalid",
  "invalid",
  "1.2Wip",
  "1.2aLpha",
  "1.2beTa",
  "1.2gamMa",
  "1.2Rc",
  "1.2.3betA2",
  "1.2.3Rc-1",
  "1.2.3rciNvalid",
  "invaLid",
  "prefix1.2.3rc1suffix"
};

char *comparetests[][2] = {
  { "1",            "0"           },
  { "2",            "1"           },
  { "1.2",          "1.2"         },
  { "1.2",          "1.3"         },
  { "1.3",          "1.2"         },
  { "1.2.0",        "1.3.0"       },
  { "1.2.0",        "1.2.1"       },
  { "1.2",          "1.2wip"      },
  { "1.2wip",       "1.2beta"     },
  { "1.2beta",      "1.2rc",      },
  { "1.2rc",        "1.2"         },
  { "1.2beta",      "1.2beta1"    },
  { "1.2beta1",     "1.2beta"     },
  { "1.2.1beta1",   "1.2beta"     },
  { "1.2beta10",    "1.2beta1"    },
  { "1.2beta10",    "1.2beta2"    },
  { "2.3",          "1.4"         },
  { "2.3",          "2.3.0"       },
  { "2.3",          "2.3.1"       },
  { "2.3.10",       "2.3.10"      },
  { "2.3.10wip",    "2.3.10beta"  },
  { "2.3.10beta",   "2.3.10beta0" },
  { "2.3.10beta0",  "2.3.10beta"  },
  { "2.3.10beta10", "2.3.10beta2" },
  { "2.3.10beta2",  "2.3.10beta10"},
  { "2.3.10beta2",  "2.3.10rc1"   },
  { "2.3.10rc2",    "2.3.10rc1"   },
  { "2.3.10rc2",    "2.3.10"      },
  { "2.3.10",       "2.3.10rc2"   },
  { "10.2",         "2.10"        },
  { "2.10",         "10.2"        }
};

int main(int argc, char *argv[])
{
  unsigned int maxlen=5;
  for (unsigned int i = 0; i < sizeof(formattests) / sizeof(*formattests); i++)
  {
    if (strlen(formattests[i]) > maxlen)
      maxlen = strlen(formattests[i]);
  }

  char *formatheaderfmt = "\n\nTesting Parsing of Version Number String\n"
                          "%*s %5s %5s %5s %5s %7s (#  ok) %8s %s\n";
  char *formattestfmt   = "%*s %5s %5d %5d %5d %7s (%2d %2s) %8d %s\n";
  printf(formatheaderfmt,
         maxlen, "input",
         "valid",
         "major",
         "minor",
         "point",
         "stage",
         "substage",
         "toString");
  for (unsigned int i = 0; i < sizeof(formattests) / sizeof(*formattests); i++)
  {
    XVersion version(formattests[i]);
    char *stagestr;
    bool unusedok;
    bool stageok;
    switch (version.stage(stageok))
    {
      case XVersion::WIP:     stagestr = "WIP";     break;
      case XVersion::ALPHA:   stagestr = "ALPHA";   break;
      case XVersion::BETA:    stagestr = "BETA";    break;
      case XVersion::RC:      stagestr = "RC";      break;
      case XVersion::FINAL:   stagestr = "FINAL";   break;
      case XVersion::UNKNOWN: stagestr = "UNKNOWN"; break;
      default:                stagestr = "default"; break;
    }

    printf(formattestfmt,
           maxlen, formattests[i],
           (version.isValid() ? "T" : "F"),
           version.majorNumber(unusedok),
           version.minorNumber(unusedok),
           version.pointNumber(unusedok),
           stagestr,
           version.stage(unusedok),
           (stageok ? "Y" : "N"),
           version.substageNumber(unusedok),
           qPrintable(version.toString()));
  }

  maxlen = 5;
  for (unsigned int i = 0; i < sizeof(comparetests) / sizeof(*comparetests); i++)
  {
    if (strlen(comparetests[i][0]) > maxlen)
      maxlen = strlen(comparetests[i][0]);
    if (strlen(comparetests[i][1]) > maxlen)
      maxlen = strlen(comparetests[i][1]);
  }
  char *compareheaderfmt = "\n\nTesting Comparison Operators\n"
                           "%*s %*s | %*s %*s  ==   >  >=   <  <=  !=\n";
  char *comparetestfmt   = "%*s %*s | %*s %*s %3s %3s %3s %3s %3s %3s\n";
  printf(compareheaderfmt,
         maxlen, "left",
         maxlen, "right",
         maxlen, "left str",
         maxlen, "right str");

  for (unsigned int i = 0; i < sizeof(comparetests) / sizeof(*comparetests); i++)
  {
    XVersion left(comparetests[i][0]);
    XVersion right(comparetests[i][1]);
    printf(comparetestfmt,
         maxlen, comparetests[i][0],
         maxlen, comparetests[i][1],
         maxlen, qPrintable(left.toString()),
         maxlen, qPrintable(right.toString()),
         (left == right ? "T" : "F"),
         (left >  right ? "T" : "F"),
         (left >= right ? "T" : "F"),
         (left <  right ? "T" : "F"),
         (left <= right ? "T" : "F"),
         (left != right ? "T" : "F"));
  }
}
