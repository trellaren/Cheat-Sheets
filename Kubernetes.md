This cheat sheet is designed for rapid reference, focusing on the most common commands and concepts you will encounter when managing an application on Kubernetes.

---

# ⚙️ Kubernetes Cheat Sheet: The Essentials

## I. 🚀 Core Concepts (The Jargon)

| Term           | What It Is                                                                                                                   | Analogy                                                                                             |
| :------------- | :--------------------------------------------------------------------------------------------------------------------------- | :-------------------------------------------------------------------------------------------------- |
| **Cluster**    | The entire set of components (Control Plane + Nodes).                                                                        | The entire factory building.                                                                        |
| **Node**       | A physical or virtual machine running the K8s software (the worker).                                                         | One specific workstation/machine in the factory.                                                    |
| **Pod**        | The smallest deployable unit. It's one or more closely related containers that share resources._Always deploy Pods._         | A single, isolated work pod holding your tools (containers).                                        |
| **Container**  | The actual application process (e.g., NGINX, Python app). Runs inside a Pod.                                                 | The tool/software running inside the work pod.                                                      |
| **Deployment** | Manages the desired state of your application (how many replicas should run and which version)._You almost always use this._ | A manager that ensures 3 workers are always assigned to Task X.                                     |
| **Service**    | An abstract layer that provides a stable network endpoint for a set of Pods, even if those Pods die or change IPs.           | The telephone extension number (it never changes) pointing to multiple people who might move desks. |
| **Namespace**  | A virtual partition within the cluster used to isolate resources and manage access control.                                  | Filing cabinets in an office, keeping departmental items separate.                                  |

---

## II. 💻 Core Commands: `kubectl` Syntax

The primary tool for interaction is `kubectl`. Always remember the basic syntax:
`kubectl <command> [resource] [flags]`

### 🔍 Viewing & Inspecting Resources

| Goal                                | Command Example                         | Explanation                                                                                |
| :---------------------------------- | :-------------------------------------- | :----------------------------------------------------------------------------------------- |
| **List all resources** (e.g., Pods) | `kubectl get pods`                      | Lists the names and statuses of all running pods.                                          |
| **List in a specific namespace**    | `kubectl get deployments -n production` | Only views deployments within the `production` namespace.                                  |
| **Show detailed status/config**     | `kubectl describe pod my-pod-name`      | Provides deep details: IP, Events (why it failed), Status history._Crucial for debugging._ |
| **Check cluster nodes**             | `kubectl get nodes`                     | Shows the health and status of your worker machines.                                       |

### 📜 Logs & Execution

| Goal                                | Command Example                             | Explanation                                                                               |
| :---------------------------------- | :------------------------------------------ | :---------------------------------------------------------------------------------------- |
| **View logs from a Pod/Deployment** | `kubectl logs my-pod-name`                  | Prints the standard output (stdout) logs for all containers in the pod.                   |
| **Follow live logs**                | `kubectl logs -f my-pod-name`               | The `-f` flag tails the logs, showing new entries in real-time.                           |
| **Execute a command inside a Pod**  | `kubectl exec -it my-pod-name -- /bin/bash` | Opens an interactive shell session (`bash`) _inside_ the running container for debugging. |

### 🛠️ Manipulation & Lifecycle

| Goal                             | Command Example                                | Explanation                                                                                          |
| :------------------------------- | :--------------------------------------------- | :--------------------------------------------------------------------------------------------------- |
| **Create a resource from YAML**  | `kubectl apply -f my-manifest.yaml`            | The standard way to deploy or update any resource using defined YAML files. (Idempotent)             |
| **Delete a specific resource**   | `kubectl delete pod my-pod-name`               | Removes the Pod._Note: Deleting a Deployment's Pod will trigger the Deployment to create a new one._ |
| **Scale an application**         | `kubectl scale deployment webapp --replicas=5` | Changes the desired number of running replicas for a specific Deployment.                            |
| **View resource history/events** | `kubectl get events`                           | Shows recent actions and changes within the cluster.                                                 |

---

## III. 🏗️ Resource Deep Dive (The Manifests)

### 🟢 1. Pod (The Unit)

A wrapper around containers. If your application has multiple cooperating services, they should live in one Pod.

- **When to use it:** For testing or for tightly coupled applications. **Avoid using this directly for production workloads.**
- **Example Use Case:** A web server container and a dedicated sidecar logging agent container running together.

### 🟡 2. Deployment (The Manager)

This is the workhorse for stateless, scalable apps. It manages the desired state: "I want 3 replicas of version X to run at all times."

- **When to use it:** Nearly always. When you need resilience and automated updates (rolling deployments).
- **Key Feature:** Handles rolling updates. If your image changes, Deployment automatically replaces old pods with new ones gradually.

### 🟠 3. Service (The Stabilizer)

Provides a stable IP address and DNS name for a group of Pods. The Pod IPs change frequently; the Service IP does not.

- **When to use it:** Every time you expose an application from inside or outside the cluster.
- **Key Concepts:**
  - `ClusterIP`: Internal IP used only within the cluster (default).
  - `NodePort`: Exposes the service on a specific port of every Node's IP address (good for testing/small environments).
  - `LoadBalancer`: Provisions an external cloud load balancer (AWS, GCP, Azure) to expose the application publicly.

---

## IV. 📜 YAML Manifest Quick Templates

All resources are defined in declarative YAML files using `kubectl apply -f`.

### Template A: The Basic Deployment (The App Code)

_(This says: "I want a deployment called `web` running 3 replicas of my container image.")_

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: web-app-deployment
spec:
  replicas: 3 # <--- Number of desired copies
  selector:
    matchLabels:
      app: web-app
  template: # Defines the Pod blueprint
    metadata:
      labels:
        app: web-app
    spec:
      containers:
        - name: nginx-container
          image: nginx:latest # <--- Your image source
          ports:
            - containerPort: 80 # The port your app listens on
```

### Template B: The Service (The Endpoint)

_(This says: "When someone hits this stable address, send them to the Pods labeled `app: web-app`.")_

```yaml
apiVersion: v1
kind: Service
metadata:
  name: web-service # <--- What you call it internally
spec:
  selector:
    app: web-app # Matches the label from your Deployment template!
  ports:
    - protocol: TCP
      port: 80 # The port the service listens on (internal)
      targetPort: 80 # Must match containerPort in Deployment
  type: LoadBalancer # <--- How it is exposed (ClusterIP, NodePort, LoadBalancer)
```

---

## V. 💡 Troubleshooting & Best Practices

| Issue/Goal                     | Command / Action                                                                            | Explanation                                                                                                                                        |
| :----------------------------- | :------------------------------------------------------------------------------------------ | :------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Cannot connect to the app?** | `kubectl describe service <svc-name>` then `kubectl get endpoint -n default`                | Check if the Service has correctly discovered any running Pod IPs. If not, your label selector is wrong.                                           |
| **App keeps crashing.**        | `kubectl logs my-pod-name --timestamps=true` followed by `kubectl describe pod my-pod-name` | Use `logs` to see application errors (e.g., code crash). Use `describe` to see Kubernetes infrastructure errors (e.g., OOMKilled, failed startup). |
| **Need external access?**      | Ensure your Service type is set to `LoadBalancer`.                                          | If running locally or on minikube, you might need `minikube service <svc-name>`.                                                                   |
| **Resource limits are hit.**   | Edit the Deployment YAML and add resource requests/limits.                                  | Always specify:`resources: limits: cpu: "100m" memory: "256Mi"` to prevent noisy neighbors from crashing the cluster.                              |
| **I need to test locally.**    | `kubectl port-forward pod my-pod 8080:80`                                                   | Maps a port on your local machine (`8080`) directly to a port inside the container (`80`). Useful for debugging without exposing it globally.      |
