#pragma once

// Wireless network configuration
#define WLAN_SSID       "<Your SSID>"    
#define WLAN_PASS       "<Your WiFi Pass>"

// Amazon AWS configuration
#define     AWS_ACCESS_KEY         "AWS Access Key"                           // Put your AWS access key here.
#define     AWS_SECRET_ACCESS_KEY  "AWS Secret Key"                           // Put your AWS secret access key here.
#define     AWS_REGION             "us-east-1"                                // The region where your SNS topic lives.                                                                                  // See the table at: http://docs.aws.amazon.com/general/latest/gr/rande.html#sns_region
#define     AWS_HOST               "sns.us-east-1.amazonaws.com"              // The host URL for the region where your SNS topic lives.                                                                                      // See the table at: http://docs.aws.amazon.com/general/latest/gr/rande.html#sns_region
#define     SNS_TOPIC_ARN          "arn%3Aaws%3Asns%3Aus-east-1%3Afoo%3Abar"  // Amazon resource name (ARN) for the SNS topic to receive notifications.
