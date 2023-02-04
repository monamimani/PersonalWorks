package main

import (
	"context"
	"fmt"
	"io"
	"log"
	"os"
	"path"
	"time"

	compute "cloud.google.com/go/compute/apiv1"
	computepb "cloud.google.com/go/compute/apiv1/computepb"
	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/mongo/readpref"

	"google.golang.org/api/iterator"
)

// Connection URI
const uri = "mongodb://Github:XD7rkisqxDjhbbXU@localhost:80/?authSource=admin&authMechanism=SCRAM-SHA-256&w=majority"

func gcloudVMInstance() {
	// projectId := "tonal-justice-374902"
	// zoneName := "us-east1-b"
	// instanceName := "mongodb-personalworks-servers-vm-0"

	// // Create a context with the Google Cloud OAuth2 token source.
	// ctx := context.Background()
	// ts, err := google.DefaultTokenSource(ctx, compute.CloudPlatformScope, iap.CloudPlatformScope)
	// if err != nil {
	// 	log.Fatalf("Failed to create token source: %v", err)
	// }

	// // Create a Compute Engine client.
	// computeClient, err := compute.NewService(ctx, option.WithTokenSource(ts))
	// if err != nil {
	// 	log.Fatalf("Failed to create Compute Engine client: %v", err)
	// }

	// // Get the IAP-secured sshForwarding instance.
	// instance, err := computeClient.Instances.Get(projectId, zoneName, instanceName).Do()
	// if err != nil {
	// 	log.Fatalf("Failed to get instance: %v", err)
	// }

	// log.Printf("Status of %s: %s", instanceName, instance.Status)

	// if instance.Status != "RUNNING" {
	// 	instancesService := compute.NewInstancesService(computeClient)
	// 	defer instancesService.Stop(projectId, zoneName, instanceName).Do()

	// 	log.Printf("Starting: %s", instanceName)
	// 	instanceStartCall :=instancesService.Start(projectId, zoneName, instanceName)
	// 	opSart, err := instanceStartCall.Do()
	// 	if err != nil {
	// 		log.Fatalf("Could not start instance: %v", err)
	// 	}

	// 	instanceStartCall.Context(ctx)
	// 	opSart.

	// 	log.Printf("Status of %s: %s", instanceName, instance.Status)
	// }

	// // Get the IAP-secured sshForwarding URL.
	// iapClient, err := iap.NewService(ctx, option.WithTokenSource(ts))
	// if err != nil {
	// 	log.Fatalf("Failed to create IAP client: %v", err)
	// }

	// resource := fmt.Sprintf("/compute/v1/projects/project-id/zones/zone/instances/%s", instanceName)
	// iapUrl, err := iapClient.V1beta1.InstanceIamPolicies.GetIamPolicy(resource).Do()
	// if err != nil {
	// 	log.Fatalf("Failed to get IAP URL: %v", err)
	// }

	// // Create the secure SSH tunnel using the IAP-secured sshForwarding URL.
	// conn, err := ssh.Dial("tcp", iapUrl.SshForwarding, &ssh.ClientConfig{
	// 	User: "ssh-username",
	// 	Auth: []ssh.AuthMethod{
	// 		ssh.PublicKeys(ssh.InsecureIgnoreHostKey()),
	// 	},
	// })
	// if err != nil {
	// 	log.Fatalf("Failed to dial SSH connection: %v", err)
	// }
	// defer conn.Close()

	// // Start the local TCP server to receive the incoming connections.
	// ln, err := net.Listen("tcp", "localhost:0")
	// if err != nil {
	// 	log.Fatalf("Failed to start local TCP server: %v", err)
	// }
	// defer ln.Close()
}

func mongodb() {
	// Create a new client and connect to the server
	opts := options.Client()
	opts.ApplyURI(uri)
	opts.SetConnectTimeout(5 * time.Second)
	client, err := mongo.Connect(context.TODO(), opts)
	if err != nil {
		log.Panic(err)
	}
	defer func() {
		if err = client.Disconnect(context.TODO()); err != nil {
			log.Panic(err)
		}
	}()
	// Ping the primary
	if err := client.Ping(context.TODO(), readpref.Primary()); err != nil {
		log.Panic(err)
	}

	databases, err := client.ListDatabaseNames(context.TODO(), bson.D{})

	if err == nil {
		fmt.Println(databases)
	}

	fmt.Println("Successfully connected and pinged.")
}

func listInstances(projectID, zone string) {
	ctx := context.Background()
	instancesClient, err := compute.NewInstancesRESTClient(ctx)
	if err != nil {
		log.Fatalf("NewInstancesRESTClient: %v", err)
	}
	defer instancesClient.Close()

	req := &computepb.ListInstancesRequest{
		Project: projectID,
		Zone:    zone,
	}

	it := instancesClient.List(ctx, req)
	log.Printf("Instances found in zone %s:", zone)
	for {
		instance, err := it.Next()
		if err == iterator.Done {
			break
		}
		if err != nil {
			log.Fatalf("Error listing instances: %v", err)
		}
		log.Printf("- %s %s", instance.GetName(), instance.GetMachineType())
	}
}

func waitForOperation(w io.Writer, projectID string, operation *compute.Operation) error {

	ctx := context.Background()

	if err := operation.Wait(ctx); err != nil {
		return fmt.Errorf("unable to wait for the operation: %v", err)
	}

	fmt.Fprintf(w, "Operation finished\n")

	return nil
}

func main() {
	projectId := "tonal-justice-374902"
	zoneName := "us-east1-b"
	instanceName := "mongodb-personalworks-servers-vm-0"

	usrHomeDir, err := os.UserHomeDir()
	if err != nil {
		log.Fatal("Can't get current user home dir.")
	}

	adcPath := path.Join(usrHomeDir, "AppData/Roaming/gcloud/application_default_credentials.json")
	fileInfo, err := os.Stat(adcPath)
	_ = fileInfo
	if err != nil {
		if os.IsNotExist(err) {
			log.Print("Google application default credentials are not setup. Run: 'gcloud auth application-default login' ")
		}
	}

	listInstances(projectId, zoneName)

	ctx := context.Background()
	instancesClient, err := compute.NewInstancesRESTClient(ctx)
	if err != nil {
		log.Fatalf("NewInstancesRESTClient: %v", err)
	}
	defer instancesClient.Close()

	getInstanceReq := &computepb.GetInstanceRequest{
		Project:  projectId,
		Zone:     zoneName,
		Instance: instanceName,
	}

	computePbInstance, err := instancesClient.Get(ctx, getInstanceReq)
	if err != nil {
		log.Fatalf("unable to start instance: %v", err)
	}
	log.Printf("Status of %s: %s", instanceName, *computePbInstance.Status)

	if (*computePbInstance.Status) != computepb.Instance_RUNNING.String() {

		startReq := &computepb.StartInstanceRequest{
			Project:  projectId,
			Zone:     zoneName,
			Instance: instanceName,
		}

		startOp, err := instancesClient.Start(ctx, startReq)
		if err != nil {
			log.Fatalf("unable to start instance: %v", err)
		}
		waitForOperation(log.Writer(), projectId, startOp)

		log.Printf("Instance started")
		computePbInstance, err := instancesClient.Get(ctx, getInstanceReq)
		if err != nil {
			log.Fatalf("unable to start instance: %v", err)
		}
		log.Printf("Status of %s: %s", instanceName, *computePbInstance.Status)
	}

	mongodb()

	// stopReq := &computepb.StopInstanceRequest{
	// 	Project:  projectId,
	// 	Zone:     zoneName,
	// 	Instance: instanceName,
	// }

	// stopOp, err := instancesClient.Stop(ctx, stopReq)
	// if err != nil {
	// 	log.Fatalf("unable to stop instance: %v", err)
	// }
	// waitForOperation(log.Writer(), projectId, stopOp)

	// log.Printf("Instance stopped")
	// computePbInstance3, err := instancesClient.Get(ctx, getInstanceReq)
	// if err != nil {
	// 	log.Fatalf("unable to start instance: %v", err)
	// }
	// log.Printf("Status of %s: %s", instanceName, *computePbInstance3.Status)

	// // Get the SSH key
	// key, err := computeService.Projects.Get(projectID).GetSerialPortOutput("serial-port-output").Do()
	// if err != nil {
	// 	fmt.Println(err)
	// }
	// sshKey := key.Contents
	// // Connect to the VM using the key
	// config := &ssh.ClientConfig{
	// 	User: instance.Username,
	// 	Auth: []ssh.AuthMethod{
	// 		ssh.PublicKeys(ssh.ParsePrivateKey([]byte(sshKey))),
	// 	},
	// 	HostKeyCallback: ssh.InsecureIgnoreHostKey(),
	// }
	// addr := fmt.Sprintf("%s:22", instance.NetworkInterfaces[0].AccessConfigs[0].NatIP)
	// client, err := ssh.Dial("tcp", addr, config)
	// if err != nil {
	// 	fmt.Println(err)
	// }
	// session, err := client.NewSession()
	// if err != nil {
	// 	fmt.Println(err)
	// }
	// defer session.Close()
	// // Run commands on the remote host
	// var b bytes.Buffer
	// session.Stdout = &b
	// if err := session.Run("ls"); err != nil {
	// 	fmt.Println("Failed to run: " + err.Error())
	// }
	// fmt.Println(b.String())
}
