
gcloud auth application-default login --impersonate-service-account=personalworks-db@tonal-justice-374902.iam.gserviceaccount.com
gcloud compute ssh mongodb-personalworks-servers-vm-0 --zone us-east1-b --project tonal-justice-374902 --tunnel-through-iap --ssh-flag="-L 27017:localhost:27017"